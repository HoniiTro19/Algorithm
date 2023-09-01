#include "libaco/aco.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;
const int max_queue_size = 10;

void producer(void) {
  aco_t *this_co = aco_get_co();
  for (int i = 0; i < 100; ++i) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]() { return q.size() < max_queue_size; });
    q.push(i);
    printf("Producer: %d\n", i);
    cv.notify_one();
    lock.unlock();
    aco_yield();
  }
}

void consumer(void) {
  aco_t *this_co = aco_get_co();
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]() { return !q.empty(); });
    int item = q.front();
    q.pop();
    printf("Consumer: %d\n", item);
    cv.notify_one();
    lock.unlock();
    aco_yield();
  }
}

struct co_pool {
  std::vector<aco_t *> pool;
  int max_size;
  int current_size;
};

co_pool *init_co_pool(int max_size) {
  co_pool *pool = new co_pool;
  pool->max_size = max_size;
  pool->current_size = 0;
  return pool;
}

void task_func(void) {
  aco_t *this_co = aco_get_co();
  // 执行任务
  aco_yield();
}

class CoroutinePool {
public:
  CoroutinePool(int max_size, aco_t *main_co)
      : max_size_(max_size), main_co_(main_co), current_size_(0) {}

  aco_t *get_coroutine() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (pool_.empty()) {
      if (current_size_ < max_size_) {
        aco_t *co = aco_create(main_co_, NULL, 0, task_func, NULL);
        ++current_size_;
        return co;
      }
    } else {
      cv_.wait(lock, [&]() { return !pool_.empty(); });
    }
    aco_t *co = pool_.front();
    pool_.pop();
    return co;
  }

  void release_coroutine(aco_t *co) {
    std::unique_lock<std::mutex> lock(mutex_);
    pool_.push(co);
    cv.notify_one();
  }

  ~CoroutinePool() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!pool_.empty()) {
      aco_t *co = pool_.front();
      pool_.pop();
      aco_destroy(co);
    }
  }

private:
  int max_size_;
  aco_t *main_co_;
  int current_size_;
  std::queue<aco_t *> pool_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

aco_t *get_co_from_pool(co_pool *pool, aco_t *creator_co, void *arg) {
  if (pool->current_size < pool->max_size) {
    aco_t *co = aco_create(creator_co, NULL, 0, task_func, arg);
    pool->pool.push_back(co);
    pool->current_size++;
    return co;
  } else {
    // ...处理协程池已满的情况，如阻塞等待或者创建新的协程...
  }
  return nullptr;
}

void release_co_to_pool(co_pool *pool, aco_t *co) {
  // 在这里可以重置协程的状态，并将其放回到协程池中以供后续使用
  // 或者将其销毁以释放资源
}

void destroy_co_pool(co_pool *pool) {
  // 销毁协程池中的所有协程，并释放相关资源
  for (aco_t *co : pool->pool) {
    aco_destroy(co);
  }
  delete pool;
  pool = nullptr;
}

int main() {
  aco_thread_init(NULL);
  aco_t *main_co = aco_create(NULL, NULL, 0, NULL, NULL);
  aco_t *producer_co = aco_create(main_co, NULL, 0, producer, NULL);
  printf("Start aco threads\n");
  aco_t *consumer_co = aco_create(main_co, NULL, 0, consumer, NULL);
  for (int i = 0; i < 100; ++i) {
    aco_resume(producer_co);
    aco_resume(consumer_co);
  }
  aco_destroy(producer_co);
  aco_destroy(consumer_co);
  aco_destroy(main_co);
  return 0;
}