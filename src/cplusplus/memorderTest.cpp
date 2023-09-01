#include "gtest/gtest.h"
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

using namespace std;

TEST(memorder, relaxed) {
  int sz = 200000;
  atomic<int> a{0};
  vector<int> observer1(sz);
  vector<int> observer2(sz);
  function<void()> func1 = [&]() {
    for (int i = 0; i < sz; ++i) {
      a.store(i, memory_order_relaxed);
    }
  };
  function<void()> func2 = [&]() {
    for (int i = 0; i < sz; ++i) {
      observer1[i] = a.load(memory_order_relaxed);
    }
  };
  function<void()> func3 = [&]() {
    for (int i = 0; i < sz; ++i) {
      observer2[i] = a.load(memory_order_relaxed);
    }
  };
  thread th1(func1);
  thread th2(func2);
  thread th3(func3);
  th1.join();
  th2.join();
  th3.join();
  ASSERT_NE(observer1, observer2);
}

TEST(memorder, seq_cls) {
  atomic<bool> x{false}, y{false};
  function<void()> func1 = [&]() { x.store(true, memory_order_seq_cst); };
  function<void()> func2 = [&]() { y.store(true, memory_order_seq_cst); };
  atomic<int> z{0};
  function<void()> func3 = [&]() {
    while (!x.load(memory_order_seq_cst))
      ;
    if (y.load(memory_order_seq_cst)) {
      ++z;
    }
  };
  function<void()> func4 = [&]() {
    while (!y.load(memory_order_seq_cst))
      ;
    if (x.load(memory_order_seq_cst)) {
      ++z;
    }
  };
  function<void()> singletest = [&]() {
    thread th1(func1), th2(func2), th3(func3), th4(func4);
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    assert(z.load() != 0);
  };
  for (int i = 0; i < 10000; ++i) {
    singletest();
  }
}

TEST(memorder, acquire_release) {
  atomic<bool> x{false}, y{false};
  function<void()> func1 = [&]() {
    x.store(true, memory_order_relaxed);
    y.store(true, memory_order_release);
  };
  function<void()> func2 = [&]() {
    while (!y.load(memory_order_acquire))
      ;
    ASSERT_TRUE(x.load(memory_order_relaxed));
  };
  function<void()> singletest = [&]() {
    thread th1(func1), th2(func2);
    th1.join();
    th2.join();
  };
  for (int i = 0; i < 10000; ++i) {
    singletest();
  }
}

TEST(memorder, release_seq) {
  vector<int> data;
  atomic<int> flag{0};
  function<void()> func1 = [&]() {
    data.push_back(42);
    flag.store(1, memory_order_release);
  };
  function<void()> func2 = [&]() {
    int expected = 1;
    while (!flag.compare_exchange_strong(expected, 2, memory_order_relaxed)) {
      expected = 1;
    }
  };
  function<void()> func3 = [&]() {
    while (flag.load(memory_order_acquire) < 2)
      ;
    ASSERT_EQ(data.back(), 42);
  };
  function<void()> singletest = [&]() {
    thread th1(func1), th2(func2), th3(func3);
    th1.join();
    th2.join();
    th3.join();
  };
  for (int i = 0; i < 10000; ++i) {
    singletest();
  }
}