#include "gtest/gtest.h"
#include "oceanbaseIntern.h"

TEST(OceanbaseIntern, solution1) {
    string ip = "127.0.0.1";
    int32_t res = OceanbaseIntern::solution1(ip);
    ASSERT_EQ(res, 2130706442);
}

TEST(OceanbaseIntern, solution2) {
    int target = 8;
    vector<int> numbers = {2, 3, 4, 1, 5, 6};
    vector<vector<int>> targetRes {{2, 1, 5}, {2, 6}, {3, 4, 1}, {3, 5}};
    
    vector<vector<int>> res;
    vector<int> curNumber;
    OceanbaseIntern::solution2(res, numbers, curNumber, 0, target);
    ASSERT_EQ(res, targetRes);
}
