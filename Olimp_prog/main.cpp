#include <bits/stdc++.h>

using namespace std;

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

struct ListNode {
     int val;
     ListNode *next;
     ListNode() : val(0), next(nullptr) {}
     ListNode(int x) : val(x), next(nullptr) {}
     ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution {
public:
    void reorderList(ListNode* head) {
        if(head->next == nullptr){
            return;
        }
        ListNode* tmp = (ListNode*) malloc(sizeof(ListNode));
        ListNode* true_tmp = (ListNode*) malloc(sizeof(ListNode));
        tmp = head->next;
        if(tmp->next == nullptr){
            return;
        }

        while(tmp->next != nullptr){
            if(tmp->next->next == nullptr){
                true_tmp = tmp->next;
                tmp->next = nullptr;
            }
            else{
                tmp = tmp->next;
            }
        }

        if(head->next != nullptr){
            true_tmp->next = head->next;
            head->next = true_tmp;

            // cout << head->next->next->val << "==\n";
            if(head->next->next != nullptr){
                reorderList(head->next->next);
            }
        }
    }
};

int main() {
    Solution sol;
    ListNode* h = (ListNode*) malloc(sizeof(ListNode));
    h->val = 1;
    ListNode* tmp = (ListNode*) malloc(sizeof(ListNode));
    tmp->val = 2;
    h->next = tmp;
    ListNode* tmp3 = (ListNode*) malloc(sizeof(ListNode));
    tmp3->val = 3;
    tmp->next = tmp3;
    ListNode* tmp4 = (ListNode*) malloc(sizeof(ListNode));
    tmp4->val = 4;
    tmp3->next = tmp4;
    ListNode* tmp5 = (ListNode*) malloc(sizeof(ListNode));
    tmp5->val = 5;
    tmp4->next = tmp5;

    sol.reorderList(h);
}
