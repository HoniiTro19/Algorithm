#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

class Solution {
private:
    std::stack<int> vstack_;
    std::vector<int> answers_;

public:
    Solution(const std::vector<int> &values) {
        answers_.assign(values.size(), 0);
        for (int i = 0; i < values.size(); ++i) {
            while (!vstack_.empty() && values[i] > values[vstack_.top()]) {
                int pos = vstack_.top();
                vstack_.pop();
                answers_[pos] = values[i];
            }
            vstack_.push(i);
        }
    }

    int calculate_query(int query) {
        return answers_[query];
    }
};

int main() {
    int value_cnt;
    int query_cnt;
    std::cin >> value_cnt >> query_cnt;
    std::vector<int> test_data;
    std::cout << value_cnt << " " << query_cnt << std::endl; 
    for (int i = 0; i < value_cnt; ++i) {
        int value;
        std::cin >> value;
        test_data.push_back(value);
    }
    Solution solution(test_data);
    for (int i = 0; i < query_cnt; ++i) {
        int query;
        std::cin >> query;
        int ans = solution.calculate_query(query);
        std::cout << ans << std::endl;
    }
}