/*
  LeetCode 23. Merge k Sorted Lists
  Сложность: Hard

  Условие:
  Дан массив из k связных списков, каждый из которых уже отсортирован
  по возрастанию. Нужно объединить их все в один отсортированный
  связный список и вернуть его.

  Идея решения (min-heap / приоритетная очередь):
  Если сливать списки последовательно по одному (list1+list2, потом
  +list3, ...), суммарная сложность будет O(k^2 * N / k) = O(k*N),
  где N — суммарное число узлов. Это не оптимально при большом k.

  Вместо этого используем min-heap (std::priority_queue) размером до k:
  1. Кладём в кучу головной узел каждого из k списков (если он не nullptr).
     Куча упорядочена по значению узла — наверху всегда узел с
     минимальным значением среди k текущих "кандидатов".
  2. На каждом шаге достаём из кучи узел с минимальным значением,
     прикрепляем его к результирующему списку и, если у извлечённого
     узла есть next, кладём этот next обратно в кучу.
  3. Повторяем, пока куча не опустеет.

  Каждый узел попадает в кучу и извлекается из неё ровно один раз,
  а операции с кучей размера k стоят O(log k).

  Сложность по времени: O(N * log k), где N — суммарное число узлов
  во всех списках, k — число списков.
  Сложность по памяти: O(k) на саму кучу (не считая результирующего
  списка, который переиспользует существующие узлы).
*/

#include <queue>
#include <vector>

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
class Solution {
public:
    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        auto compare = [](const ListNode* a, const ListNode* b) {
            return a->val > b->val;
        };
        std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(compare)> heap(compare);

        for (ListNode* head : lists) {
            if (head != nullptr) {
                heap.push(head);
            }
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (!heap.empty()) {
            ListNode* smallest = heap.top();
            heap.pop();

            tail->next = smallest;
            tail = smallest;

            if (smallest->next != nullptr) {
                heap.push(smallest->next);
            }
        }

        tail->next = nullptr;
        return dummy.next;
    }
};
