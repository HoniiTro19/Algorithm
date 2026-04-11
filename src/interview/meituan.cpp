#include "gtest/gtest.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct A {
    char a;
    int b;
    char c;
    double d;
};

struct alignas(16) B : A {
    uint16_t e;
    virtual ~B() {};
};

TEST(meituan, solution) {
    A a;
    ASSERT_EQ(24, sizeof(A));
    B b;
    ASSERT_EQ(88, sizeof(B));
}