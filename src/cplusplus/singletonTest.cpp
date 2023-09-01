#include "gtest/gtest.h"
#include <atomic>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

// 线程安全的单例模式
class Singleton {
public:
  // aggresively add lock(actually locks are only needed when creating the
  // static instance)
  static Singleton &getInstance() {
    lock_guard<mutex> lock(mutex_);
    if (instance_ == nullptr) {
      instance_ = new Singleton();
      static Destructor helper;
    }
    return *instance_;
  }

  static Singleton &getAtomicInstance() {
    auto *ainstance = ainstance_.load(memory_order_acquire);
    if (!ainstance) {
      lock_guard<mutex> lock(mutex_);
      if (!(ainstance = ainstance_.load(memory_order_relaxed))) {
        ainstance = new Singleton();
        ainstance_.store(ainstance, memory_order_release);
      }
    }
    return *ainstance;
  }

  bool getTrue() { return true; }

private:
  Singleton() {}
  ~Singleton() {}
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
  class Destructor {
  public:
    ~Destructor() {
      if (Singleton::instance_) {
        delete Singleton::instance_;
        Singleton::instance_ = nullptr;
      }
    }
  };
  static mutex mutex_;
  static Singleton *instance_;
  static atomic<Singleton *> ainstance_;
};

Singleton *Singleton::instance_ = nullptr;
mutex Singleton::mutex_;

TEST(Singleton, getInstance) {
  Singleton &singleton = Singleton::getInstance();
  EXPECT_TRUE(singleton.getTrue());
}