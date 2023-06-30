// #include "sort.h"

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

template<class T>
void InsertSort<T>::solution(vector<T> &vec) {
    int len = vec.size();
    for (int i = 1; i < len; ++i) {
        int insert = vec[i];
        for (int j = i - 1; j >= 0; --j) {
            if (vec[j] > insert) {
                vec[j + 1] = vec[j];
                vec[j] = insert;
            } else {
                break;
            }
        }
    }
}

template<class T>
void MergeSort<T>::solution(vector<T> &vec) {
    int len = vec.size();
    vector<T> place(len);
    for (int seg = 1; seg < len; seg += seg) {
        for (int start = 0; start < len; start += 2 * seg) {
            int low = start;
            int mid = min(start + seg, len);
            int high = min(start + 2 * seg, len);
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            int cur = low;
            while (start1 < end1 && start2 < end2) {
                place[cur++] = vec[start1] < vec[start2] ? vec[start1++] : vec[start2++];
            }
            while (start1 < end1) {
                place[cur++] = vec[start1++];
            }
            while (start2 < end2) {
                place[cur++] = vec[start2++];
            }
        }
        place.swap(vec);
    }
}

template<class T>
void QuickSort<T>::solution(vector<T> &vec, int start, int end) {
    if (start >= end) {
        return;
    }
    T mid = vec[end];
    int left = start;
    int right = end - 1;
    while (left < right) {
        while (vec[left] < mid && left < right) {
            ++left;
        }
        while (vec[right] >= mid && left < right) {
            --right;
        }
        swap(vec[left], vec[right]);
    }
    if (vec[left] >= vec[end]) {
        swap(vec[left], vec[end]);
    }
    solution(vec, start, left);
    solution(vec, left + 1, end);
}

// template class BubbleSort<int>;
// template class InsertSort<int>;
// template class MergeSort<int>;
// template class QuickSort<int>;