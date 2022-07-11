#include <vector>
#include <string>

#ifndef SORT_H_
#define SORT_H_

using namespace std;

template<class T>
class BubbleSort {
public:
    BubbleSort();
    ~BubbleSort();
    void solution(vector<T> &vec);
};

#endif