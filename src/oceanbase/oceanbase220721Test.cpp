#include "gtest/gtest.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class OceanBase {
public:
  static int32_t solution1(string ip);
  static void solution2(vector<vector<int>> &res, vector<int> &numbers,
                        vector<int> &curNumber, int index, int remain);
};

int32_t OceanBase::solution1(string ip) {
  int32_t decode = 0;
  int token = 0;
  for (char ch : ip) {
    if (ch != '.') {
      token += ch - '0';
      token *= 10;
    } else {
      token /= 10;
      // memcpy or plus, both ok
      memcpy(&decode, &token, 1);
      // decode += token / 10;
      decode <<= 8;
      token = 0;
    }
  }
  // decode += token;
  memcpy(&decode, &token, 1);
  return decode;
}

void OceanBase::solution2(vector<vector<int>> &res, vector<int> &numbers,
                          vector<int> &curNumber, int index, int remain) {
  if (remain == 0) {
    res.push_back(curNumber);
    return;
  } else if (remain < 0) {
    return;
  }

  for (int i = index; i < numbers.size(); ++i) {
    curNumber.push_back(numbers[i]);
    solution2(res, numbers, curNumber, i + 1, remain - numbers[i]);
    curNumber.pop_back();
  }
}

TEST(OceanBase, solution1) {
  string ip = "127.0.0.1";
  int32_t res = OceanBase::solution1(ip);
  ASSERT_EQ(res, 2130706442);
}

TEST(OceanBase, solution2) {
  int target = 8;
  vector<int> numbers = {2, 3, 4, 1, 5, 6};
  vector<vector<int>> targetRes{{2, 1, 5}, {2, 6}, {3, 4, 1}, {3, 5}};

  vector<vector<int>> res;
  vector<int> curNumber;
  OceanBase::solution2(res, numbers, curNumber, 0, target);
  ASSERT_EQ(res, targetRes);
}
