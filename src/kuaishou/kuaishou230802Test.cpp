#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

using namespace std;

void backtrace(vector<vector<int>> &ans, vector<int> &nums,
               vector<int> &factors, int left, int idx) {
  if (left == 1) {
    ans.push_back(nums);
  }
  for (int i = idx; i < factors.size(); ++i) {
    if (factors[i] > left) {
      break;
    }
    nums.push_back(factors[i]);
    backtrace(ans, nums, factors, left / factors[i], i);
    nums.pop_back();
  }
}

TEST(Kuaishou, solution) {
  int N = 32;
  vector<vector<int>> ans;
  vector<int> nums;
  vector<int> factors;
  for (int i = 2; i < N; ++i) {
    if (N % i == 0) {
      factors.push_back(i);
    }
  }
  backtrace(ans, nums, factors, N, 0);
  vector<vector<int>> target_ans = {{2, 2, 2, 2, 2}, {2, 2, 2, 4}, {2, 2, 8},
                                    {2, 4, 4},       {2, 16},      {4, 8}};
  ASSERT_EQ(target_ans, ans);
}

class MyClass : public enable_shared_from_this<MyClass> {
public:
  MyClass() {}
  ~MyClass() {}
  shared_ptr<MyClass> getSharedPtr() { return shared_ptr<MyClass>(this); }

  shared_ptr<MyClass> sharedPtrFromThis() { return shared_from_this(); }

  shared_ptr<MyClass> moveSharedPtr() {
    return move(shared_ptr<MyClass>(this));
  }
};

TEST(Kuaishou, getsharedptr) {
  // shared_ptr<MyClass> objPtr = make_shared<MyClass>();
  shared_ptr<MyClass> objPtr(new MyClass());
  // 有问题
  // shared_ptr<MyClass> sharedPtr = objPtr->getSharedPtr();
  shared_ptr<MyClass> sharedPtr = objPtr->sharedPtrFromThis();
  // shared_ptr<MyClass> sharedPtr = objPtr->moveSharedPtr();
  int uc1 = objPtr.use_count();
  int uc2 = sharedPtr.use_count();
  // ASSERT_EQ(objPtr.use_count(), 1);
  // ASSERT_EQ(sharedPtr.use_count(), 1);
}