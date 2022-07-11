#include "sort.h"

template<class T>
BubbleSort<T>::BubbleSort() {

}

template<class T>
BubbleSort<T>::~BubbleSort() {

}

template<class T>
void BubbleSort<T>::solution(vector<T> &vec) {
    int len = vec.size();
    bool orderly = false;
    for (int i = 0; i < len - 1 && !orderly; ++i) {
        orderly = true;
        for (int j = 0; j < len - 1 - i; ++j) {
            if (vec[j] > vec[j + 1]) {
                orderly = false;
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

template class BubbleSort<int>;