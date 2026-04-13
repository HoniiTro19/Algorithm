#include <string>
#include <vector>
#include <iostream>

class Solution {
public:
    bool solution(const std::string &str, const std::string &like) {
        std::vector<std::string> parts;
        std::string part;
        for (int i = 0; i < like.size(); ++i) {
            if (like[i] == '%') {
                parts.push_back(part);
                part.clear();
            } else {
                part += like[i];
            }
        }
        parts.push_back(part);
        if (parts.size() == 1) {
            return str == like;
        }

        int pos = 0;
        int end = str.size();

        const std::string &first = parts.front();
        if (!first.empty()) {
            if (str.size() < first.size() || str.compare(0, first.size(), first) != 0) {
                return false;
            }
            pos = first.size();
        }

        const std::string &last = parts.back();
        if (!last.empty()) {
            if (str.size() < last.size() || str.compare(str.size() - last.size(), last.size(), last) != 0) {
                return false;
            }
            end = str.size() - last.size();
        }
        // csdc%dsadsa%dsadas%dsad
        for (int i = 1; i < parts.size() - 1; ++i) {
            const std::string &part = parts[i];
            if (part.empty()) {
                continue;
            }
            int index = str.find(part, pos);
            if (index == std::string::npos || (index + parts.size()) > end) {
                return false;
            }
            pos = index + part.size();
        }

        return pos <= end;
    }
};

int main() {
    Solution solution;
    std::string test_oracle("abbc");
    std::string like0("%bb%");
    std::string like1("a%d");
    std::string like2("ab%");
    std::string like3("a%c");
    std::string like4("ab%c");
    std::string like5("a%cb");
    std::string like6("%bc");
    std::string like7("a%a");
    std::cout << "%bb%: " << solution.solution(test_oracle, like0) << std::endl;
    std::cout << "a%d: " << solution.solution(test_oracle, like1) << std::endl;
    std::cout << "ab%: " << solution.solution(test_oracle, like2) << std::endl;
    std::cout << "a%c: " << solution.solution(test_oracle, like3) << std::endl;
    std::cout << "ab%c: " << solution.solution(test_oracle, like4) << std::endl;
    std::cout << "a%cb: " << solution.solution(test_oracle, like5) << std::endl;
    std::cout << "%bc: " << solution.solution(test_oracle, like6) << std::endl;
    std::cout << "a%a: " << solution.solution(test_oracle, like7) << std::endl;
    return 0;
}