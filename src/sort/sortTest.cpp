#include <algorithm>
#include <chrono>
#include <random>

#include "sort.h"
#include "gtest/gtest.h"

int testSize = 1000;
int loopTimes = 1000;
vector<int> orderly(testSize);
vector<int> disorderly(testSize);

TEST(BubbleSort, bubbleSortRandomShuffle) {
  for (int i = 0; i < testSize; ++i) {
    orderly[i] = i + 1;
    disorderly[i] = i + 1;
  }
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  for (int i = 0; i < loopTimes; ++i) {
    shuffle(disorderly.begin(), disorderly.end(), default_random_engine(seed));
    BubbleSort<int>::solution(disorderly);
    EXPECT_EQ(orderly, disorderly);
  }
}

TEST(InsertSort, insertSortRandomShuffle) {
  for (int i = 0; i < testSize; ++i) {
    orderly[i] = i + 1;
    disorderly[i] = i + 1;
  }
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  for (int i = 0; i < loopTimes; ++i) {
    shuffle(disorderly.begin(), disorderly.end(), default_random_engine(seed));
    InsertSort<int>::solution(disorderly);
    EXPECT_EQ(orderly, disorderly);
  }
}

TEST(MergeSort, mergeSortRandomShuffle) {
  for (int i = 0; i < testSize; ++i) {
    orderly[i] = i + 1;
    disorderly[i] = i + 1;
  }
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  for (int i = 0; i < loopTimes; ++i) {
    shuffle(disorderly.begin(), disorderly.end(), default_random_engine(seed));
    MergeSort<int>::solution(disorderly);
    EXPECT_EQ(orderly, disorderly);
  }
}

TEST(QuickSort, quickSortRandomShuffle) {
  for (int i = 0; i < testSize; ++i) {
    orderly[i] = i + 1;
    disorderly[i] = i + 1;
  }
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  for (int i = 0; i < loopTimes; ++i) {
    shuffle(disorderly.begin(), disorderly.end(), default_random_engine(seed));
    QuickSort<int>::solution(disorderly, 0, disorderly.size() - 1);
    EXPECT_EQ(orderly, disorderly);
  }
}

TEST(HeapSort, heapSortRandomShuffle) {
  for (int i = 0; i < testSize; ++i) {
    orderly[i] = i + 1;
    disorderly[i] = i + 1;
  }
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  for (int i = 0; i < loopTimes; ++i) {
    shuffle(disorderly.begin(), disorderly.end(), default_random_engine(seed));
    HeapSort<int>::solution(disorderly);
    EXPECT_EQ(orderly, disorderly);
  }
}