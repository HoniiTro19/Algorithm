#include "tecent.h"
#include "gtest/gtest.h"

TEST(Tecent, solution) {
  Tecent tecent = Tecent();
  std::vector<std::vector<int>> matrix = {
      {0, 1, 2, 3, 4}, {5, 6, 7, 8, 9}, {10, 11, 12, 13, 14}};
  std::pair<int, int> res = tecent.solution(matrix, 8);
  std::pair<int, int> truth(1, 3);
  ASSERT_EQ(res, truth);
}