#include "leetcode.h"
#include "gtest/gtest.h"

TEST(Leetcode, rearCreateList) {
  vector<int> test_data = {1, 2, 3, 4, 5};
  ListNode *head = new ListNode();
  head = RearCreateList(test_data, head);
  EXPECT_EQ(test_data, SerializeList(head));
  DestroyList(head);
}

TEST(Leetcode, headCreateList) {
  vector<int> test_data = {5, 4, 3, 2, 1};
  vector<int> target_res = {1, 2, 3, 4, 5};
  ListNode *head = new ListNode();
  head = HeadCreateList(test_data, head);
  EXPECT_EQ(target_res, SerializeList(head));
  DestroyList(head);
}

TEST(Leetcode, createTree) {
  vector<int> test_data = {1,  2, -1, 3,  4,  -1, -1, 5,
                           -1, 6, -1, -1, -1, -1, -1};
  vector<int> target_res = {1, 2, 3, 5, 4, 6};
  TreeNode *root = new TreeNode();
  root = CreateTreeFull(test_data, root);
  EXPECT_EQ(target_res, PreorderSerializeTree(root));
  DestroyTree(root);
}

TEST(Leetcode, solution1) {
  vector<int> test_data = {2, 7, 11, 15};
  vector<int> target_res = {0, 1};
  vector<int> res = Leetcode::Solution1(test_data, 9);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution49) {
  vector<string> test_data = {"eat", "tea", "tan", "ate", "nat", "bat"};
  vector<vector<string>> target_res = {
      {"bat"}, {"tan", "nat"}, {"eat", "tea", "ate"}};
  vector<vector<string>> res = Leetcode::Solution49(test_data);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution11) {
  vector<int> test_data = {1, 8, 6, 2, 5, 4, 8, 3, 7};
  int target_res = 49;
  int res = Leetcode::Solution11(test_data);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution3) {
  string test_data = "tmmzuxt";
  int target_res = 5;
  int res = Leetcode::Solution3(test_data);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution560) {
  vector<int> test_data = {1, 2, 1, 2, 1};
  int target_res = 4;
  int res = Leetcode::Solution560(test_data, 3);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution53) {
  vector<int> test_data = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  int target_res = 6;
  int res = Leetcode::Solution53(test_data);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution25) {
  vector<int> test_data_vec = {1, 2, 3, 4, 5};
  vector<int> target_res_vec = {2, 1, 4, 3, 5};
  ListNode *test_data_list = new ListNode();
  ListNode *target_res_list = new ListNode();
  test_data_list = RearCreateList(test_data_vec, test_data_list);
  target_res_list = RearCreateList(target_res_vec, target_res_list);
  ListNode *res_list = Leetcode::Solution25(test_data_list, 2);
  EXPECT_EQ(SerializeList(target_res_list), SerializeList(res_list));
  DestroyList(test_data_list);
  DestroyList(target_res_list);
}

TEST(Leetcode, solution102) {
  vector<int> test_data_vec = {3, 9, 20, -1, -1, 15, 7};
  vector<vector<int>> target_res_vec = {{3}, {9, 20}, {15, 7}};
  TreeNode *root = new TreeNode();
  root = CreateTreeFull(test_data_vec, root);
  vector<vector<int>> res = Leetcode::Solution102(root);
  EXPECT_EQ(target_res_vec, res);
  DestroyTree(root);
}

TEST(Leetcode, solution146) {
  vector<string> ops = {"LRUCache", "put", "put", "get", "put",
                        "get",      "put", "get", "get", "get"};
  vector<vector<int>> params = {{2}, {1, 1}, {2, 2}, {1}, {3, 3},
                                {2}, {4, 4}, {1},    {3}, {4}};
  vector<int> target_res = {-2, -2, -2, 1, -2, -1, -2, -1, 3, 4};
  vector<int> res = Leetcode::Solution146(ops, params);
  ASSERT_EQ(target_res, res);
}