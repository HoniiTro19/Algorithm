#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#ifndef SORT_H_
#define SORT_H_

using namespace std;

template <class T> class BubbleSort {
public:
  static void solution(vector<T> &vec);
};

template <class T> class InsertSort {
public:
  static void solution(vector<T> &vec);
};

template <class T> class MergeSort {
public:
  static void solution(vector<T> &vec);
};

template <class T> class QuickSort {
public:
  static void solution(vector<T> &vec, int start, int end);
};

template <class T> class HeapSort {
public:
  static void solution(vector<T> &vec);
};

class ExternalSort {
public:
  void mergeSort(const vector<pair<char *, size_t>> &blocks, FILE *output);

private:
  void sortBlock(char *buffer, size_t size);

private:
  const int BLOCK_SIZE = 1 << 28;
  const int BUFFER_SIZE = 1 << 26;
  const int MERGE_BUFFER_SIZE = 1 << 30;
};
#endif

#include "sort.ipp"