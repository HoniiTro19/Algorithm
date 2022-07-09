#include "gtest/gtest.h"
#include "demo.h"

class Demo;
namespace {
    Demo demo;
    const vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    TEST(Demo, solution) {
        vector<string> sol = demo.solution();
        ASSERT_EQ(msg.size(), sol.size());
        EXPECT_EQ(msg, demo.solution());
    }
}
