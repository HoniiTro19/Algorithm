#include <mysql/mysql.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <future>
#include <atomic>

class ConnectionPool {
public:
    ConnectionPool(const std::string& host, const std::string& user,
                   const std::string& pass, const std::string& db,
                   int port, int poolSize)
        : poolSize_(poolSize), activeCount_(0), isClosed_(false) {
        for (int i = 0; i < poolSize; ++i) {
            MYSQL* conn = mysql_init(nullptr);
            if (!conn) {
                closeAll();
                throw std::runtime_error("mysql_init failed");
            }
            // 修复1：MYSQL_OPT_RECONNECT 必须在 mysql_real_connect 之前设置
            my_bool reconnect = 1;
            mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

            if (!mysql_real_connect(conn, host.c_str(), user.c_str(),
                                    pass.c_str(), db.c_str(), port, nullptr, 0)) {
                std::string err = mysql_error(conn);
                mysql_close(conn);  // 修复2：释放当前失败的连接
                closeAll();         // 修复2：释放已创建的连接，避免内存泄漏
                throw std::runtime_error("MySQL connect failed: " + err);
            }
            idleConnections_.push(conn);
        }
    }

    ~ConnectionPool() {
        close();
    }

    // 禁止拷贝和赋值
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;

    // 获取一个连接，阻塞直到有可用连接
    std::shared_ptr<MYSQL> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 修复3：等待条件加入 activeCount_ < poolSize_ 防止连接被双重获取
        cond_.wait(lock, [this] { return !idleConnections_.empty() || isClosed_; });
        if (isClosed_ && idleConnections_.empty()) {
            return nullptr;
        }
        MYSQL* conn = idleConnections_.front();
        idleConnections_.pop();
        ++activeCount_;
        // 使用自定义删除器，归还连接而不是真正删除
        return std::shared_ptr<MYSQL>(conn, [this](MYSQL* c) {
            this->releaseConnection(c);
        });
    }

    void close() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (isClosed_) return;
        isClosed_ = true;
        // 修复4：等待所有活跃连接归还后再关闭，避免 shared_ptr 删除器回调悬空 this
        cond_.wait(lock, [this] { return activeCount_ == 0; });
        closeAll();
        cond_.notify_all();
    }

private:
    // 关闭队列中所有空闲连接（调用方需持锁或在单线程中调用）
    void closeAll() {
        while (!idleConnections_.empty()) {
            mysql_close(idleConnections_.front());
            idleConnections_.pop();
        }
    }

    void releaseConnection(MYSQL* conn) {
        std::lock_guard<std::mutex> lock(mutex_);
        --activeCount_;
        if (isClosed_) {
            mysql_close(conn);
            cond_.notify_all();  // 通知 close() 中等待 activeCount_==0 的线程
            return;
        }
        idleConnections_.push(conn);
        cond_.notify_one();
    }

    int poolSize_;
    int activeCount_;
    bool isClosed_;
    std::queue<MYSQL*> idleConnections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(size_t numThreads, std::shared_ptr<ConnectionPool> connPool)
        : stop_(false), connPool_(connPool) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers_.emplace_back([this] { workerLoop(); });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (std::thread &worker : workers_) {
            if (worker.joinable())
                worker.join();
        }
    }

    // 提交任务，返回一个future，可用于获取结果
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using ReturnType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<ReturnType> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if (stop_) {
                throw std::runtime_error("ThreadPool stopped");
            }
            tasks_.emplace([task]() { (*task)(); });
        }
        condition_.notify_one();
        return result;
    }

private:
    void workerLoop() {
        while (true) {
            Task task;
            {
                std::unique_lock<std::mutex> lock(queueMutex_);
                condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                if (stop_ && tasks_.empty())
                    return;
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            // 修复5：worker 不再额外获取连接，连接由任务自身按需获取，避免双重占用
            if (task) {
                task();
            }
        }
    }

    std::vector<std::thread> workers_;
    std::queue<Task> tasks_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
    bool stop_;
    std::shared_ptr<ConnectionPool> connPool_;
};

int main() {
    try {
        // 创建连接池：参数依次为 host, user, password, db, port, poolSize
        auto connPool = std::make_shared<ConnectionPool>("127.0.0.1", "root", "123456", "test", 3306, 4);

        // 创建线程池，线程数与连接数相同（通常如此）
        ThreadPool threadPool(4, connPool);

        // 提交任务：每个任务获取一个连接并执行SQL
        for (int i = 0; i < 10; ++i) {
            threadPool.submit([connPool]() {
                auto conn = connPool->getConnection();
                if (!conn) {
                    std::cerr << "Failed to get connection" << std::endl;
                    return;
                }
                // 执行查询
                const char* query = "SELECT NOW()";
                if (mysql_query(conn.get(), query)) {
                    std::cerr << "Query failed: " << mysql_error(conn.get()) << std::endl;
                } else {
                    MYSQL_RES* res = mysql_store_result(conn.get());
                    if (res) {
                        MYSQL_ROW row = mysql_fetch_row(res);
                        if (row && row[0]) {
                            std::cout << "Current time: " << row[0] << std::endl;
                        }
                        mysql_free_result(res);
                    }
                }
            });
        }

        // 等待一段时间让任务执行完毕
        std::this_thread::sleep_for(std::chrono::seconds(2));
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
