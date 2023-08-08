
#include "gtest/gtest.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <cstring>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int solution(int n, vector<int> A) {
  sort(A.begin(), A.end());
  vector<int> vecn;
  while (n) {
    vecn.push_back(n % 10);
    n /= 10;
  }
  reverse(vecn.begin(), vecn.end());
  int ans = 0;
  int idxn;
  for (idxn = 0; idxn < vecn.size(); ++idxn) {
    int idxA;
    for (idxA = 0; idxA < A.size(); ++idxA) {
      if (vecn[idxn] < A[idxA]) {
        break;
      }
    }
    if (idxA > 0) {
      ans *= 10;
      ans += A[idxA - 1];
      if (vecn[idxn] > A[idxA - 1]) {
        ++idxn;
        break;
      }
    } else {
      break;
    }
  }
  if (idxn == 0)
    return -1;
  for (; idxn < vecn.size(); ++idxn) {
    ans *= 10;
    ans += A.back();
  }
  return ans;
}

TEST(Bytedance, solution) {
  vector<int> testn = {23121, 93121, 11111, 78932};
  vector<vector<int>> testA = {{2, 4, 9}, {2, 4, 9}, {2, 4, 9}, {2, 4, 9}};
  vector<int> target_res = {22999, 92999, -1, 49999};
  for (int i = 0; i < testn.size(); ++i) {
    int res = solution(testn[i], testA[i]);
    ASSERT_EQ(target_res[i], res);
  }
}