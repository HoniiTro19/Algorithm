#include <functional>
#include <iostream>
#include <vector>

class Tecent {
public:
  Tecent() {}
  ~Tecent() {}
  std::pair<int, int> solution(const std::vector<std::vector<int>> &matrix,
                               int target);
};

int upper_bound(int start, int end, int target,
                std::function<int(int)> get_data) {
  int mid;
  while (start < end) {
    mid = (start + end) / 2;
    if (get_data(mid) <= target) {
      start = mid + 1;
    } else {
      end = mid;
    }
  }
  return start;
}

int lower_bound(int start, int end, int target,
                std::function<int(int)> get_data) {
  int mid;
  while (start < end) {
    mid = (start + end) / 2;
    if (get_data(mid) >= target) {
      end = mid;
    } else {
      start = mid + 1;
    }
  }
  return start;
}

std::pair<int, int>
Tecent::solution(const std::vector<std::vector<int>> &matrix, int target) {
  std::function<int(int, int)> get_data = [&](int x, int y) -> int {
    return matrix[x][y];
  };
  int res_x = lower_bound(0, matrix.size(), target,
                          std::bind(get_data, std::placeholders::_1, 0));
  res_x--;
  int res_y = lower_bound(0, matrix[0].size(), target,
                          std::bind(get_data, res_x, std::placeholders::_1));
  return {res_x, res_y};
}
