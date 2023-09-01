#include "gtest/gtest.h"
#include <string>
#include <vector>

using namespace std;

bool solutionstr(const string &str) {
  int right = str.size() - 1;
  int left = 0;
  while (left <= right) {
    if (str[left++] != str[right--]) {
      return false;
    }
  }
  return true;
}

bool solutionvec(const vector<char> &str) {
  int right = str.size() - 1;
  int left = 0;
  while (left <= right) {
    if (str[left++] != str[right--]) {
      return false;
    }
  }
  return true;
}

TEST(ali, solution) {
  string test_str = "asdsa";
  vector<char> test_vec = {'a', 's', 'd', 's', 'a'};
  EXPECT_EQ(true, solutionstr(test_str));
  ASSERT_EQ(true, solutionvec(test_vec));
}