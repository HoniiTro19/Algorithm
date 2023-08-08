#include "gtest/gtest.h"
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

// 线程安全的单例模式
class Singleton {
public:
  static Singleton &getInstance() {
    lock_guard<mutex> lock(mutex_);
    if (instance_ == nullptr) {
      instance_ = new Singleton();
      static Destructor helper;
    }
    return *instance_;
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
};

Singleton *Singleton::instance_ = nullptr;
mutex Singleton::mutex_;

TEST(Singleton, getInstance) {
  Singleton &singleton = Singleton::getInstance();
  EXPECT_TRUE(singleton.getTrue());
}