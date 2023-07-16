#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <numeric>
#include <queue>
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
  return head;
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

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

TreeNode *CreateSubtree(const vector<int> &seq, int index) {
  if (index >= seq.size() || -1 == seq[index])
    return nullptr;
  TreeNode *node = new TreeNode(seq[index]);
  node->left = CreateSubtree(seq, index * 2 + 1);
  node->right = CreateSubtree(seq, index * 2 + 2);
  return node;
}

TreeNode *CreateTreeFull(const vector<int> &seq, TreeNode *root) {
  int index = 0;
  if (index >= seq.size() || -1 == seq[index])
    return root;
  root->val = seq[index];
  root->left = CreateSubtree(seq, index * 2 + 1);
  root->right = CreateSubtree(seq, index * 2 + 2);
  return root;
}

void DestroyTree(TreeNode *root) {
  if (nullptr == root)
    return;
  DestroyTree(root->left);
  DestroyTree(root->right);
  delete root;
}

void PreorderSerializeSubtree(vector<int> &serialize, TreeNode *root) {
  if (nullptr == root) {
    return;
  }
  serialize.push_back(root->val);
  PreorderSerializeSubtree(serialize, root->left);
  PreorderSerializeSubtree(serialize, root->right);
}

vector<int> PreorderSerializeTree(TreeNode *root) {
  vector<int> serialize;
  if (nullptr == root) {
    serialize.push_back(-1);
    return serialize;
  }
  serialize.push_back(root->val);
  PreorderSerializeSubtree(serialize, root->left);
  PreorderSerializeSubtree(serialize, root->right);
  return serialize;
}

class Leetcode {
public:
  Leetcode() {}
  ~Leetcode() {}
  // 两数之和
  static vector<int> Solution1(const vector<int> &nums, int target);
  // 字母异位词分组
  static vector<vector<string>> Solution49(const vector<string> &strs);
  // 盛最多水的容器
  static int Solution11(const vector<int> &height);
  // 无重复字符的最长子串
  static int Solution3(string s);
  // 和为K的子数组
  static int Solution560(const vector<int> &nums, int k);
  // 最大子数组和
  static int Solution53(const vector<int> &nums);
  // K个一组翻转链表
  static ListNode *Solution25(ListNode *head, int k);
  // 二叉树的层序遍历
  static vector<vector<int>> Solution102(TreeNode *root);
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

ListNode *Leetcode::Solution25(ListNode *head, int k) {
  int n = 0;
  for (ListNode *cur = head->next; cur; cur = cur->next) {
    ++n;
  }
  int cnt = 0;
  ListNode *phead = head;
  ListNode *pcur = head->next;
  ListNode *ppre = nullptr;
  ListNode *pnext = nullptr;
  for (; n >= k; n -= k) {
    for (int i = 0; i < k; ++i) {
      pnext = pcur->next;
      pcur->next = ppre;
      ppre = pcur;
      pcur = pnext;
      ++cnt;
    }
    cnt = 0;
    pnext = phead->next;
    phead->next->next = pcur;
    phead->next = ppre;
    phead = pnext;
  }
  return head;
}

vector<vector<int>> Leetcode::Solution102(TreeNode *root) {
  queue<TreeNode *> levelq;
  vector<vector<int>> levels;
  if (nullptr != root) {
    levelq.push(root);
    int idx = 0;
    while (!levelq.empty()) {
      int sz = levelq.size();
      levels.push_back({});
      for (int i = 0; i < sz; ++i) {
        TreeNode *tmp = levelq.front();
        levels[idx].push_back(tmp->val);
        levelq.pop();
        if (tmp->left)
          levelq.push(tmp->left);
        if (tmp->right)
          levelq.push(tmp->right);
      }
      ++idx;
    }
  }
  return levels;
}

#endif