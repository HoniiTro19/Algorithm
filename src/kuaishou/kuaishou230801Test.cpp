#include <algorithm>
#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

double calculate(double a, double b, char op) {
  switch (op) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    return a / b;
  default:
    return 0;
  }
}

int get_priority(char op) {
  switch (op) {
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  default:
    return 0;
  }
}

double solution(string infix) {
  vector<double> stackrpn;
  vector<char> stackop;
  int sz = infix.size();
  int idx = 0;
  auto get_number = [&]() -> int {
    int res = 0;
    while (idx < sz) {
      if (isdigit(infix[idx])) {
        res *= 10;
        res += infix[idx] - '0';
        ++idx;
      } else {
        break;
      }
    }
    return res;
  };

  while (idx < sz) {
    if (isdigit(infix[idx])) {
      int num = get_number();
      stackrpn.push_back((double)num);
    } else {
      while (!stackop.empty() &&
             get_priority(infix[idx]) <= get_priority(stackop.back())) {
        char op = stackop.back();
        stackop.pop_back();
        double data1 = stackrpn.back();
        stackrpn.pop_back();
        double data2 = stackrpn.back();
        stackrpn.pop_back();
        stackrpn.push_back(calculate(data2, data1, op));
      }
      stackop.push_back(infix[idx]);
      ++idx;
    }
  }

  double ans = 0;
  while (!stackop.empty()) {
    char op = stackop.back();
    stackop.pop_back();
    double data1 = stackrpn.back();
    stackrpn.pop_back();
    double data2 = stackrpn.back();
    stackrpn.pop_back();
    stackrpn.push_back(calculate(data2, data1, op));
  }
  return stackrpn.back();
}

TEST(Kuaishou, solution) {
  string test_data = "1+2/3-4/5";
  double ans = solution(test_data);
  EXPECT_TRUE(abs(ans - 0.86667) < 0.01);
}