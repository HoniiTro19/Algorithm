#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

using namespace std;

// 自旋锁要保证unlock操作synchronizes-with lock操作
class Spinlock {
public:
  // 外部两次lock操作不能保证顺序
  void lock() {
    while (flag.exchange(true, memory_order_acquire))
      ;
    cout << "thread: " << this_thread::get_id() << " get lock" << endl;
  }

  void unlock() {
    flag.store(false, memory_order_release);
    cout << "thread: " << this_thread::get_id() << " release lock" << endl;
  }

private:
  atomic<bool> flag{false};
};

int main() {
  Spinlock lock;
  function<void()> func = [&]() {
    lock.lock();
    this_thread::sleep_for(chrono::seconds(3));
    lock.unlock();
  };
  thread th1(func), th2(func);
  th1.join();
  th2.join();
  return 0;
}