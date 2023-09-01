#include <algorithm>
#include <atomic>
#include <functional>
#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <vector>

using namespace std;

template <class Obj> class LockFreeStack {
public:
  Obj *Pop() {
    while (true) {
      Obj *ret_ptr = top_ptr.load();
      if (!ret_ptr)
        return nullptr;
      Obj *next_ptr = ret_ptr->next;
      if (top_ptr.compare_exchange_weak(ret_ptr, next_ptr)) {
        return ret_ptr;
      }
    }
  }
  void Push(Obj *obj_ptr) {
    while (true) {
      Obj *next_ptr = top_ptr.load();
      obj_ptr->next = next_ptr;
      if (top_ptr.compare_exchange_weak(next_ptr, obj_ptr)) {
        return;
      }
    }
  }

private:
  atomic<Obj *> top_ptr{nullptr};
};

struct ListNode {
  int value;
  ListNode *next;
  ListNode(int value) : value(value) {}
};

TEST(LockFreeStack, Pop) {
  LockFreeStack<ListNode> stk;
  vector<int> res;
  function<void()> func1 = [&]() {
    for (int i = 0; i < 100; ++i) {
      ListNode *node = new ListNode(i);
      stk.Push(node);
    }
  };
  function<vector<int>()> func2 = [&]() -> vector<int> {
    for (int i = 0; i < 100; ++i) {
      ListNode *tmp = stk.Pop();
      res.push_back(tmp->value);
      delete tmp;
    }
    return res;
  };
  thread th1(func1);
  thread th2(func2);
  th1.join();
  th2.join();
  vector<int> target_res(100);
  iota(target_res.begin(), target_res.end(), 0);
  sort(res.begin(), res.end());
  ASSERT_EQ(target_res, res);
}

// template <class Obj> class LockFreeList {
// public:
//   void pushFront(Obj *value) {
//     while (true) {
//     }
//   }

// private:
//   atomic<Obj *> head;
//   atomic<Obj *> tail;
// };