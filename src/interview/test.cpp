#include <iostream>
#include <gtest/gtest.h>
#include <vector>

struct ListNode {
  int val{0};
  ListNode *next{nullptr};
  ListNode *random{nullptr};
};

class Solution {
public:
  static ListNode *copyList(ListNode *head) {
    ListNode *curr = head;
    while (curr) {
      ListNode *next = curr->next;
      ListNode *new_node = new ListNode();
      new_node->val = curr->val;
      curr->next = new_node;
      new_node->next = next;
      curr = next;
    }

    curr = head;
    while (curr) {
      ListNode *new_node = curr->next;
      new_node->random = curr->random->next;
      curr = new_node->next;
    }

    ListNode dummy;
    ListNode *prev = &dummy;
    curr = head;
    while (curr) {
      ListNode *new_node = curr->next;
      curr->next = new_node->next;
      prev->next = new_node;
      curr = curr->next;
      prev = new_node;
    }
    return dummy.next;
  }
};

TEST(Solution, copyList) {
  std::vector<ListNode*> nodes(8, nullptr);
  for (int i = 0; i < 8; ++i) {
    nodes[i] = new ListNode();
    nodes[i]->val = i;
  }
  for (int i = 0; i < 7; ++i) {
    nodes[i]->next = nodes[i + 1];
  }
  for (int i = 0; i < 8; ++i) 
  {
    nodes[i]->random = nodes[i % 4];
  }
  ListNode *copy = Solution::copyList(nodes[0]);
  for (int i = 0; i < 8; ++i) {
    ASSERT_NE(copy, nullptr);
    ASSERT_EQ(copy->val, nodes[i]->val);
    ASSERT_NE(copy->random, nodes[i]->random);
    ASSERT_EQ(copy->random->val, nodes[i]->random->val);
    copy = copy->next;
  }
}