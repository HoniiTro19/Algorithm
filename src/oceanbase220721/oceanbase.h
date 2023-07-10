#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#ifndef OCEANBASE_INTERN_
#define OCEANBASE_INTERN_

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
#endif