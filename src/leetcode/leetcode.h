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

ListNode *HeadCreateList(const vector<int> &seq) {
  ListNode *head = new ListNode();
  ListNode *pnew = nullptr;
  for (int v : seq) {
    pnew = new ListNode(v);
    pnew->next = head->next;
    head->next = pnew;
  }
  return head;
}

ListNode *RearCreateList(const vector<int> &seq) {
  ListNode *head = new ListNode();
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

TreeNode *CreateTreeFull(const vector<int> &seq) {
  int n = seq.size();
  assert((n & (n + 1)) == 0);
  vector<TreeNode *> tree(n);
  for (int i = 0; i < n; ++i) {
    tree[i] = seq[i] == -1 ? nullptr : new TreeNode(seq[i]);
  }
  for (int i = 0; i < n / 2; ++i) {
    if (tree[i] == nullptr)
      continue;
    tree[i]->left = tree[i * 2 + 1];
    tree[i]->right = tree[i * 2 + 2];
  }
  return tree[0];
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
  // LRU缓存
  static vector<int> Solution146(vector<string> &ops,
                                 vector<vector<int>> &params);
  static int Solution200(vector<vector<char>> &grid);
  // 搜索旋转排序数组
  static int Solution33(vector<int> &nums, int target);
  // 在排序数组中查找元素的第一个和最后一个位置
  static vector<int> Solution34(vector<int> &nums, int target);
  // 二叉树的最近公共祖先
  static TreeNode *Solution236(TreeNode *root, TreeNode *p, TreeNode *q);
  // 环形链表Ⅱ
  static ListNode *Solution142(ListNode *head);
  // 反转链表
  static ListNode *Solution206(ListNode *head);
  // 合并两个有序链表
  static ListNode *Solution21(ListNode *list1, ListNode *list2);
  // 最小栈
  static vector<string> Solution155(vector<string> &ops,
                                    vector<vector<int>> &params);
  // 最长连续序列
  // 三数之和
  // 找到字符串中所有字母异位词
  // 两两交换链表中的节点
  // 删除链表的倒数第N个结点
  // 有效的括号
  // 字符串解码
  // 每日温度
  static vector<int> Solution739(vector<int> &temperatures);
  // 数组中的第K个最大元素
  static int Solution215(vector<int> &nums, int k);
  // 全排列
  static vector<vector<int>> Solution46(vector<int> &nums);
  // 反转链表Ⅱ
  static ListNode *Solution92(ListNode *head, int left, int right);
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
    }
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

struct LRUHandle {
  int key;
  int value;
  LRUHandle *prev;
  LRUHandle *next;

  LRUHandle() : key(0), value(0), prev(nullptr), next(nullptr) {}
  LRUHandle(int key, int value)
      : key(key), value(value), prev(nullptr), next(nullptr) {}
  LRUHandle(int key, int value, LRUHandle *prev, LRUHandle *next)
      : key(key), value(value), prev(prev), next(next) {}
};

class LRUCache {
public:
  LRUCache(size_t capacity);
  int get(int key);
  void put(int key, int value);

private:
  void lru_remove(LRUHandle *handle);
  void lru_append(LRUHandle *list, LRUHandle *handle);

private:
  size_t capacity_;
  size_t usage_;
  LRUHandle lru_;
  unordered_map<int, LRUHandle *> cache_;
};

LRUCache::LRUCache(size_t capacity) : capacity_(capacity), usage_(0) {
  lru_.next = &lru_;
  lru_.prev = &lru_;
}

void LRUCache::lru_remove(LRUHandle *handle) {
  handle->prev->next = handle->next;
  handle->next->prev = handle->prev;
  --usage_;
}

void LRUCache::lru_append(LRUHandle *list, LRUHandle *handle) {
  list->prev->next = handle;
  handle->prev = list->prev;
  handle->next = list;
  list->prev = handle;
  ++usage_;
}

int LRUCache::get(int key) {
  auto gethandle = cache_.find(key);
  if (cache_.end() != gethandle) {
    LRUHandle *handle = gethandle->second;
    lru_remove(handle);
    lru_append(&lru_, handle);
    return handle->value;
  } else {
    return -1;
  }
}

void LRUCache::put(int key, int value) {
  auto gethandle = cache_.find(key);
  if (cache_.end() != gethandle) {
    LRUHandle *oldhandle = gethandle->second;
    lru_remove(oldhandle);
  }
  LRUHandle *newhandle = new LRUHandle(key, value);
  cache_[key] = newhandle;
  lru_append(&lru_, newhandle);
  if (usage_ > capacity_) {
    cache_.erase(lru_.next->key);
    lru_remove(lru_.next);
  }
}

vector<int> Leetcode::Solution146(vector<string> &ops,
                                  vector<vector<int>> &params) {
  LRUCache *lru = nullptr;
  vector<int> res;
  for (int i = 0; i < ops.size(); ++i) {
    if ("LRUCache" == ops[i]) {
      assert(1 == params[i].size());
      lru = new LRUCache(params[i][0]);
      res.push_back(-2);
    } else if ("get" == ops[i]) {
      assert(nullptr != lru);
      assert(1 == params[i].size());
      res.push_back(lru->get(params[i][0]));
    } else if ("put" == ops[i]) {
      assert(2 == params[i].size());
      lru->put(params[i][0], params[i][1]);
      res.push_back(-2);
    } else {
      assert(-1 == 0);
    }
  }
  return res;
}

void markisland(vector<vector<char>> &grid, int x, int y) {
  int m = grid.size();
  int n = grid[0].size();
  if (x < 0 || x >= m || y < 0 || y >= n || '0' == grid[x][y])
    return;
  grid[x][y] = '0';
  markisland(grid, x + 1, y);
  markisland(grid, x - 1, y);
  markisland(grid, x, y + 1);
  markisland(grid, x, y - 1);
}

int Leetcode::Solution200(vector<vector<char>> &grid) {
  int m = grid.size();
  int n = grid[0].size();
  int res = 0;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if ('1' == grid[i][j]) {
        ++res;
        markisland(grid, i, j);
      }
    }
  }
  return res;
}

int Leetcode::Solution33(vector<int> &nums, int target) {
  int n = nums.size();
  int left = 0;
  int right = n;
  int mid = 0;
  if (n < 1)
    return -1;
  if (nums[0] == target)
    return 0;
  while (left < right) {
    mid = (left + right) / 2;
    if (nums[mid] == target)
      return mid;
    if (nums[0] < nums[mid]) {
      if (nums[0] < target && target < nums[mid]) {
        right = mid;
      } else {
        left = mid + 1;
      }
    } else {
      if (nums[mid] < target && target <= nums[n - 1]) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
  }
  return -1;
}

vector<int> Leetcode::Solution34(vector<int> &nums, int target) {
  auto lower = [&](vector<int> nums, int target) -> int {
    int left = 0;
    int right = nums.size();
    int mid = 0;
    while (left < right) {
      mid = (left + right) / 2;
      if (nums[mid] < target) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
    return left < nums.size() && nums[left] == target ? left : -1;
  };

  auto upper = [&](vector<int> nums, int target) -> int {
    int left = 0;
    int right = nums.size();
    int mid = 0;
    while (left < right) {
      mid = (left + right) / 2;
      if (nums[mid] <= target) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
    return 0 < left && nums[left - 1] == target ? left - 1 : -1;
  };
  return {lower(nums, target), upper(nums, target)};
}

bool ancestorBacktrace(TreeNode *root, TreeNode *p, TreeNode *q,
                       TreeNode *&res) {
  if (root == nullptr)
    return false;
  bool lchild = ancestorBacktrace(root->left, p, q, res);
  bool rchild = ancestorBacktrace(root->right, p, q, res);
  if ((lchild && rchild) || ((lchild || rchild) && (root == p || root == q))) {
    res = root;
  }
  return lchild || rchild || root == p || root == q;
}

TreeNode *Leetcode::Solution236(TreeNode *root, TreeNode *p, TreeNode *q) {
  TreeNode *res = nullptr;
  ancestorBacktrace(root, p, q, res);
  return res;
}

ListNode *Leetcode::Solution142(ListNode *head) {
  ListNode *fast = head;
  ListNode *slow = head;
  // f = 2s; f = s + nb -> s = nb; f = 2nb
  // f = a; s = a + nb
  do {
    if (slow->next) {
      slow = slow->next;
    } else {
      return nullptr;
    }
    if (fast->next && fast->next->next) {
      fast = fast->next->next;
    } else {
      return nullptr;
    }
  } while (fast != slow);
  fast = head;
  while (fast != slow) {
    fast = fast->next;
    slow = slow->next;
  }
  return slow;
}

ListNode *Leetcode::Solution206(ListNode *head) {
  ListNode *prev = nullptr;
  ListNode *cur = head->next;
  ListNode *nxt = nullptr;
  while (cur) {
    nxt = cur->next;
    cur->next = prev;
    prev = cur;
    cur = nxt;
  }
  head->next = prev;
  return head;
}

ListNode *Leetcode::Solution21(ListNode *list1, ListNode *list2) {
  ListNode *head = new ListNode();
  list1 = list1->next;
  list2 = list2->next;
  ListNode *list3 = head;
  while (list1 && list2) {
    if (list1->val < list2->val) {
      list3->next = list1;
      list1 = list1->next;
      list3 = list3->next;
    } else {
      list3->next = list2;
      list2 = list2->next;
      list3 = list3->next;
    }
  }
  if (list1) {
    list3->next = list1;
  }
  if (list2) {
    list3->next = list2;
  }
  return head;
}

class MinStack {
public:
  MinStack();
  void push(int val);
  void pop();
  int top();
  int getMin();

private:
  vector<int> values;
  vector<int> minvalues;
};

MinStack::MinStack() {}

void MinStack::push(int val) {
  values.push_back(val);
  int minn = minvalues.size() ? min(minvalues.back(), val) : val;
  minvalues.push_back(minn);
}

void MinStack::pop() {
  values.pop_back();
  minvalues.pop_back();
}

int MinStack::top() { return values.back(); }

int MinStack::getMin() { return minvalues.back(); }

vector<string> Leetcode::Solution155(vector<string> &ops,
                                     vector<vector<int>> &params) {
  int n = ops.size();
  vector<string> res(ops.size(), "null");
  MinStack *minStack = nullptr;
  for (int i = 0; i < n; ++i) {
    if ("MinStack" == ops[i]) {
      assert(i == 0);
      minStack = new MinStack();
    } else if ("push" == ops[i]) {
      assert(minStack != nullptr);
      assert(params[i].size() == 1);
      minStack->push(params[i][0]);
    } else if ("pop" == ops[i]) {
      assert(minStack != nullptr);
      minStack->pop();
    } else if ("top" == ops[i]) {
      assert(minStack != nullptr);
      res[i] = to_string(minStack->top());
    } else {
      assert(minStack != nullptr);
      res[i] = to_string(minStack->getMin());
    }
  }
  return res;
}

vector<int> Leetcode::Solution739(vector<int> &temperatures) {
  int sz = temperatures.size();
  vector<int> stacktemp;
  vector<int> res(sz, 0);
  int back;
  for (int i = 0; i < sz; ++i) {
    while (stacktemp.size()) {
      back = stacktemp.back();
      if (temperatures[i] <= temperatures[back])
        break;
      res[back] = i - back;
      stacktemp.pop_back();
    }
    stacktemp.push_back(i);
  }
  return res;
}

int Leetcode::Solution215(vector<int> &nums, int k) {
  int n = nums.size();
  auto swim = [&](int i) {
    int left = (i - 1) / 2;
    while (i > 0 && nums[i] < nums[left]) {
      swap(nums[i], nums[left]);
      i = left;
      left = (i - 1) / 2;
    }
  };

  auto sink = [&](int i, int k) {
    int j = 2 * i + 1;
    while (j < k) {
      if (j + 1 < k && nums[j + 1] < nums[j]) {
        ++j;
      }
      if (nums[i] > nums[j]) {
        swap(nums[i], nums[j]);
        i = j;
        j = 2 * i + 1;
      } else {
        break;
      }
    }
  };

  for (int i = 0; i < k; ++i) {
    swim(i);
  }
  for (int i = k; i < n; ++i) {
    if (nums[i] > nums[0]) {
      swap(nums[i], nums[0]);
      sink(0, k);
    }
    cout << nums[0] << " " << nums[i] << endl;
  }
  return nums[0];
}

void backtrace46(vector<vector<int>> &ans, vector<int> &nums, int i, int len) {
  if (i == len) {
    ans.push_back(nums);
    return;
  }
  for (int j = i; j < len; ++j) {
    swap(nums[i], nums[j]);
    backtrace46(ans, nums, i + 1, len);
    swap(nums[i], nums[j]);
  }
}

vector<vector<int>> Leetcode::Solution46(vector<int> &nums) {
  vector<vector<int>> ans;
  backtrace46(ans, nums, 0, nums.size());
  return ans;
}

ListNode *Leetcode::Solution92(ListNode *head, int left, int right) {
  ListNode *guard = head;
  for (int i = 1; i < left; ++i) {
    guard = guard->next;
  }
  ListNode *cur = guard->next;
  ListNode *nxt;
  ListNode *tail = cur;
  for (int i = left; i <= right; ++i) {
    nxt = cur->next;
    cur->next = guard->next;
    guard->next = cur;
    cur = nxt;
  }
  tail->next = cur;
  return head->next;
}
#endif