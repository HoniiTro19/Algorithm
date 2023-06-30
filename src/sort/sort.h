#include <vector>

#ifndef SORT_H_
#define SORT_H_

using namespace std;

template<class T>
class BubbleSort {
public:
    static void solution(vector<T> &vec);
};

template<class T>
class InsertSort {
public:
    static void solution(vector<T> &vec);
};

template<class T>
class MergeSort {
public:
    static void solution(vector<T> &vec);
};

template<class T>
class QuickSort {
public:
    static void solution(vector<T> &vec, int start, int end);
};

#endif

#include "sort.ipp"