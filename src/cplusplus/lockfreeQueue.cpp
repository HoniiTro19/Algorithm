#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

template<typename T>
class LockFreeQueue {
private:
  struct Node {
    T data;
    std::shared_ptr<Node> next;
    Node(const T &value) : data(value), next(nullptr) {}
  };

  std::shared_ptr<Node> head;
  std::shared_ptr<Node> tail;

public:
  LockFreeQueue() {
    auto dummy = std::make_shared<Node>(T{});
    std::atomic_store(&head, dummy);
    std::atomic_store(&tail, dummy);
  }

  LockFreeQueue(const LockFreeQueue &other) = delete;
  LockFreeQueue &operator=(const LockFreeQueue &other) = delete;
  ~LockFreeQueue() {
    T unused;
    while (pop(unused)) {}
  }

  void push(const T &value) {
    auto new_node = std::make_shared<Node>(value);
    std::shared_ptr<Node> old_tail;
    std::shared_ptr<Node> null_next;
    
    for (;;) {
      old_tail = std::atomic_load(&tail);
      std::shared_ptr<Node> next;
      if (old_tail) {
        next = std::atomic_load(&old_tail->next);
      }
      if (old_tail != std::atomic_load(&tail)) {
        continue;
      }
      if (next) {
        std::atomic_compare_exchange_weak(&tail, &old_tail, next);
        continue;
      }
      if (std::atomic_compare_exchange_weak(&old_tail->next, &null_next, new_node)) {
        std::atomic_compare_exchange_weak(&tail, &old_tail, new_node);
        return;
      }
    }
  }

  bool pop(T &result) {
    std::shared_ptr<Node> old_head;
    std::shared_ptr<Node> next;
    for (;;) {
      old_head = std::atomic_load(&head);
      if (old_head) {
        next = std::atomic_load(&old_head->next);
      }
      if (!next) {
        return false;
      }
      if (std::atomic_compare_exchange_weak(&head, &old_head, next)) {
        result = std::move(next->data);
        return true;
      }
    }
  }
};

int main() {
  LockFreeQueue<int> lock_free_queue;
  std::thread producer([&]() {
    for (int i = 0; i < 100; ++i) {
      lock_free_queue.push(i);
    }
  });

  std::thread consumer([&]() {
    int value;
    for (int i = 0; i < 100; ++i) {
      while (!lock_free_queue.pop(value)) {}
      std::cout << value << std::endl;
    }
  });
  producer.join();
  consumer.join();
  return 0;
}