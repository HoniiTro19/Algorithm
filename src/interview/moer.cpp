#include <iostream>
#include <vector>
#include <stack>

class QuickSort {
public:
    static void solution(std::vector<int> &vec) {
        size_t start = 0;
        size_t end = vec.size() - 1;
        std::stack<size_t> vstk;
        int pivot = 0;
        vstk.push(start);
        vstk.push(end);
        while (!vstk.empty()) {
            size_t right = vstk.top();
            vstk.pop();
            size_t left = vstk.top();
            vstk.pop();
            pivot = vec[right];
            size_t l = left;
            size_t r = right - 1;
            while (l < r) {
                while (vec[l] < pivot && l < r) {
                    ++l;
                }
                while (vec[r] > pivot && l < r) {
                    --r;
                }
                std::swap(vec[l], vec[r]);
            }
            if (vec[l] >= pivot) {
                std::swap(vec[l], vec[right]);
            }
            if (left < l) {
                vstk.push(left);
                vstk.push(l);
            }
            if (right > l + 1) {
                vstk.push(l + 1);
                vstk.push(right);
            }
        }
    }
};

int main() {
    std::vector<int> test_oracle{1, 3, 5, 7, 9, 2, 4, 6, 8};
    QuickSort::solution(test_oracle);
    for (int val : test_oracle) {
        std::cout << val << std::endl;
    }
    return 0;
}