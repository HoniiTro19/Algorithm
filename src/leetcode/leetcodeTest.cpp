#include "leetcode.h"
#include "gtest/gtest.h"

TEST(Leetcode, rearCreateList) {
  vector<int> test_data = {1, 2, 3, 4, 5};
  ListNode *head = RearCreateList(test_data);
  EXPECT_EQ(test_data, SerializeList(head));
  DestroyList(head);
}

TEST(Leetcode, headCreateList) {
  vector<int> test_data = {5, 4, 3, 2, 1};
  vector<int> target_res = {1, 2, 3, 4, 5};
  ListNode *head = HeadCreateList(test_data);
  EXPECT_EQ(target_res, SerializeList(head));
  DestroyList(head);
}

TEST(Leetcode, createTree) {
  vector<int> test_data = {1,  2, -1, 3,  4,  -1, -1, 5,
                           -1, 6, -1, -1, -1, -1, -1};
  vector<int> target_res = {1, 2, 3, 5, 4, 6};
  TreeNode *root = CreateTreeFull(test_data);
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
  ListNode *test_data_list = RearCreateList(test_data_vec);
  ListNode *target_res_list = RearCreateList(target_res_vec);
  ListNode *res_list = Leetcode::Solution25(test_data_list, 2);
  EXPECT_EQ(SerializeList(target_res_list), SerializeList(res_list));
  DestroyList(test_data_list);
  DestroyList(target_res_list);
}

TEST(Leetcode, solution102) {
  vector<int> test_data_vec = {3, 9, 20, -1, -1, 15, 7};
  vector<vector<int>> target_res_vec = {{3}, {9, 20}, {15, 7}};
  TreeNode *root = CreateTreeFull(test_data_vec);
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

TEST(Leetcode, solution200) {
  vector<vector<char>> test_data = {{'1', '1', '1', '1', '0'},
                                    {'1', '1', '0', '1', '0'},
                                    {'1', '1', '0', '0', '0'},
                                    {'0', '0', '0', '0', '0'}};
  int target_res = 1;
  int res = Leetcode::Solution200(test_data);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution33) {
  vector<int> test_data = {5, 1, 3};
  int target_res = 2;
  int res = Leetcode::Solution33(test_data, 3);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution236) {
  vector<int> test_data = {1, 2, 3, 4, -1, -1, -1};
  int target_res = 2;
  TreeNode *root = CreateTreeFull(test_data);
  TreeNode *p = root->left;
  TreeNode *q = root->left->left;
  TreeNode *res = Leetcode::Solution236(root, p, q);
  EXPECT_EQ(target_res, res->val);
  DestroyTree(root);
}

TEST(Leetcode, solution142) {
  vector<int> test_data = {3, 2, 0, -4};
  int target_res = 2;
  ListNode *head = RearCreateList(test_data);
  ListNode *tail = head;
  for (int i = 0; i < test_data.size(); ++i) {
    tail = tail->next;
  }
  tail->next = head->next->next;
  ListNode *res = Leetcode::Solution142(head);
  int res_int = res->val;
  EXPECT_EQ(target_res, res_int);
  // TODO DestroyList is not suitable for circular linked list
  // DestroyList(head);
}

TEST(Leetcode, solution206) {
  vector<int> test_data = {1, 2, 3, 4, 5};
  vector<int> target_res = {5, 4, 3, 2, 1};
  ListNode *head = RearCreateList(test_data);
  ListNode *res = Leetcode::Solution206(head);
  EXPECT_EQ(target_res, SerializeList(res));
  DestroyList(head);
}

TEST(Leetcode, solution21) {
  vector<int> test_data1 = {1, 3, 5, 7, 9};
  vector<int> test_data2 = {2, 4, 6, 8, 10};
  ListNode *head1 = RearCreateList(test_data1);
  ListNode *head2 = RearCreateList(test_data2);
  vector<int> target_res = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  ListNode *res = Leetcode::Solution21(head1, head2);
  EXPECT_EQ(target_res, SerializeList(res));
  DestroyList(res);
}

TEST(Leetcode, solution155) {
  vector<string> test_ops = {"MinStack", "push", "push", "push",
                             "getMin",   "pop",  "top",  "getMin"};
  vector<vector<int>> test_params = {{}, {-2}, {0}, {-3}, {}, {}, {}, {}};
  vector<string> target_res = {"null", "null", "null", "null",
                               "-3",   "null", "0",    "-2"};
  vector<string> res = Leetcode::Solution155(test_ops, test_params);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution739) {
  vector<int> test_data = {73, 74, 75, 71, 69, 72, 76, 73};
  vector<int> target_res = {1, 1, 4, 2, 1, 1, 0, 0};
  vector<int> res = Leetcode::Solution739(test_data);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution215) {
  vector<int> test_data = {3, 2, 1, 5, 6, 4};
  int target_res = 5;
  int res = Leetcode::Solution215(test_data, 2);
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution46) {
  vector<int> test_data = {1, 2, 3};
  vector<vector<int>> target_res = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3},
                                    {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};
  vector<vector<int>> res = Leetcode::Solution46(test_data);
  sort(target_res.begin(), target_res.end());
  sort(res.begin(), res.end());
  ASSERT_EQ(target_res, res);
}

TEST(Leetcode, solution92) {
  vector<int> test_data = {1, 2, 3, 4, 5};
  ListNode *head = RearCreateList(test_data);
  vector<int> target_res = {1, 4, 3, 2, 5};
  ListNode *res = Leetcode::Solution92(head, 2, 4);
  EXPECT_EQ(target_res, SerializeList(res));
  DestroyList(head);
}