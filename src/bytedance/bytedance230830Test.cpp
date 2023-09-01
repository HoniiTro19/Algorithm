#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
  int val;
  ListNode *pre;
  ListNode *next;
  ListNode(int val) : val(val), pre(nullptr), next(nullptr) {}
};

ListNode *rearCreateList(const vector<int> &vec) {
  ListNode *phead = new ListNode(0);
  ListNode *pend = phead;
  for (int val : vec) {
    ListNode *pnew = new ListNode(val);
    pend->next = pnew;
    pnew->pre = pend;
    pend = pnew;
  }
  return phead->next;
}

ListNode *reverseList(ListNode *phead) {
  ListNode *pcur = phead;
  ListNode *pnext = nullptr;
  ListNode *ppre = nullptr;
  while (pcur) {
    pnext = pcur->next;
    pcur->next = ppre;
    pcur->pre = pnext;
    ppre = pcur;
    pcur = pnext;
  }
  return ppre;
}

int main() {
  vector<int> test_vec = {1, 2, 3, 4, 5};
  ListNode *phead = rearCreateList(test_vec);
  phead = reverseList(phead);
  ListNode *pcur = phead;
  while (pcur) {
    cout << pcur->val << " ";
    pcur = pcur->next;
  }
  cout << endl;
  return 0;
}