#include <algorithm>
#include <chrono>
#include <random>

#include "sort.h"
#include "gtest/gtest.h"

int testSize = 100;
int loopTimes = 10;
vector<int> orderly(testSize);
vector<int> disorderly(testSize);

TEST(BubbleSort, randomShuffle) {
    BubbleSort<int> bubbleSort;
    for (int i = 0; i < testSize; ++i) {
        orderly[i] = i + 1;
        disorderly[i] = i + 1;    
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    for (int i = 0; i < loopTimes; ++i) {
        shuffle(disorderly.begin(), disorderly.end(), default_random_engine(seed));
        bubbleSort.solution(disorderly);
        EXPECT_EQ(orderly, disorderly);
    }
}
