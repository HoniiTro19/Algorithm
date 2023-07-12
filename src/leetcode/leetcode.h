#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef _LEETCODE_
#define _LEETCODE_

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode *HeadCreateList(const vector<int> &seq, ListNode *head) {
  ListNode *pnew = nullptr;
  for (int v : seq) {
    pnew = new ListNode(v);
    pnew->next = head->next;
    head->next = pnew;
  }
  return head;
}

ListNode *RearCreateList(const vector<int> &seq, ListNode *head) {
  ListNode *pnew = nullptr;
  ListNode *pend = head;
  for (int v : seq) {
    pnew = new ListNode(v);
    pend->next = pnew;
    pend = pnew;
  }
}

void DestroyList(ListNode *head) {
  ListNode *next = head->next;
  while (next) {
    delete head;
    head = next;
    next = head->next;
  }
  delete head;
}

vector<int> SerializeList(const ListNode *head) {
  vector<int> serialize;
  ListNode *cur = head->next;
  while (cur) {
    serialize.push_back(cur->val);
    cur = cur->next;
  }
  return serialize;
}

class Leetcode {
public:
  Leetcode() {}
  ~Leetcode() {}
  static vector<int> Solution1(const vector<int> &nums, int target);
  static vector<vector<string>> Solution49(const vector<string> &strs);
  static int Solution11(const vector<int> &height);
  static int Solution3(string s);
  static int Solution560(const vector<int> &nums, int k);
  static int Solution53(const vector<int> &nums);
  static ListNode *Solution25(ListNode *head, int k);
};

vector<int> Leetcode::Solution1(const vector<int> &nums, int target) {
  unordered_map<int, int> existence;
  int sz = nums.size();
  for (int i = 0; i < sz; ++i) {
    auto iter = existence.find(nums[i]);
    if (existence.end() != iter) {
      return {iter->second, i};
    }
    existence.insert({target - nums[i], i});
  }
  return {-1, -1};
}

vector<vector<string>> Leetcode::Solution49(const vector<string> &strs) {
  // NOTE generic hash function for int array?
  // auto arrayhash = [fn = hash<int>{}](const array<int, 26> &arr) -> size_t {
  //   return accumulate(arr.begin(), arr.end(), 0u, [&](size_t acc, int num) {
  //     return (acc << 1) ^ fn(num);
  //   });
  // };

  // NOTE define hash function for unordered_map
  // unordered_map<array<int, 26>, vector<string>, delctype(arrayhash)>
  // group_res(0, arrayhash);
  unordered_map<int, vector<string>> group_res;
  auto hashfunc = [](const string &str) -> int {
    vector<int> cnts(26);
    for (const char &ch : str)
      ++cnts[ch - 'a'];
    std::size_t seed = cnts.size();
    for (const int &i : cnts)
      seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  };
  for (const string &str : strs) {
    group_res[hashfunc(str)].emplace_back(move(str));
  }
  vector<vector<string>> res;
  for (auto group : group_res) {
    res.emplace_back(move(group.second));
  }
  return res;
}

int Leetcode::Solution11(const vector<int> &height) {
  int left = 0;
  int right = height.size() - 1;
  int res = -1;
  while (left < right) {
    int bottom = right - left;
    if (height[left] <= height[right]) {
      res = max(res, bottom * height[left]);
      ++left;
    } else {
      res = max(res, bottom * height[right]);
      --right;
    }
  }
  return res;
}

int Leetcode::Solution3(string s) {
  unordered_set<char> charset;
  int left = 0;
  int res = 0;
  for (int right = 0; right < s.size(); ++right) {
    if (charset.end() != charset.find(s[right])) {
      while (s[left] != s[right]) {
        charset.erase(s[left]);
        ++left;
      }
      charset.erase(s[left++]);
    }
    charset.insert(s[right]);
    res = max(res, right - left + 1);
  }
  return res;
}

int Leetcode::Solution560(const vector<int> &nums, int k) {
  int res = 0, sum = 0;
  unordered_map<int, int> prefix;
  prefix[0] = 1;
  for (int i = 0; i < nums.size(); ++i) {
    sum += nums[i];
    res += prefix[sum - k];
    ++prefix[sum];
  }
  return res;
}

int Leetcode::Solution53(const vector<int> &nums) {
  int dp = 0, res = 0;
  int maxmin = INT_MIN;
  for (int i = 0; i < nums.size(); ++i) {
    if (nums[i] > 0 || dp + nums[i] > 0) {
      dp += nums[i];
    } else {
      maxmin = max(maxmin, nums[i]);
      dp = 0;
    }
    res = max(res, dp);
  }
  return res == 0 ? maxmin : res;
}

ListNode *Leetcode::Solution25(ListNode *head, int k) {}

#endif