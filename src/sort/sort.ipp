template <class T> void BubbleSort<T>::solution(vector<T> &vec) {
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

template <class T> void InsertSort<T>::solution(vector<T> &vec) {
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

template <class T> void MergeSort<T>::solution(vector<T> &vec) {
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
        place[cur++] =
            vec[start1] < vec[start2] ? vec[start1++] : vec[start2++];
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

template <class T>
void QuickSort<T>::solution(vector<T> &vec, int start, int end) {
  // 非递归版本
  stack<int> stk;
  stk.push(start);
  stk.push(end);
  T pivot;
  while (!stk.empty()) {
    int right = stk.top();
    stk.pop();
    int left = stk.top();
    stk.pop();
    pivot = vec[right];
    int l = left;
    int r = right - 1;
    while (l < r) {
      while (vec[l] < pivot && l < r) {
        ++l;
      }
      while (vec[r] > pivot && l < r) {
        --r;
      }
      swap(vec[l], vec[r]);
    }
    if (vec[l] >= pivot) {
      swap(vec[l], vec[right]);
    }
    if (left < l) {
      stk.push(left);
      stk.push(l);
    }
    if (right > l + 1) {
      stk.push(l + 1);
      stk.push(right);
    }
  }
  // 递归版本
  // if (start >= end) {
  //   return;
  // }
  // T pivot = vec[end];
  // int left = start;
  // int right = end - 1;
  // while (left < right) {
  //   while (vec[left] < pivot && left < right) {
  //     ++left;
  //   }
  //   while (vec[right] > pivot && left < right) {
  //     --right;
  //   }
  //   swap(vec[left], vec[right]);
  // }
  // if (vec[left] >= vec[end]) {
  //   swap(vec[left], vec[end]);
  // }
  // solution(vec, start, left);
  // solution(vec, left + 1, end);
}

template <class T> void HeapSort<T>::solution(vector<T> &vec) {
  int n = vec.size();
  auto swim = [=, &vec](int i) {
    int parent = (i - 1) / 2;
    while (i > 0 && vec[i] > vec[parent]) {
      swap(vec[i], vec[parent]);
      i = parent;
      parent = (i - 1) / 2;
    }
  };

  auto sink = [=, &vec](int i, int sz) {
    int left = i * 2 + 1;
    while (left < sz) {
      if (left + 1 < sz && vec[left + 1] > vec[left]) {
        ++left;
      }
      if (vec[left] > vec[i]) {
        swap(vec[i], vec[left]);
        i = left;
        left = i * 2 + 1;
      } else {
        break;
      }
    }
  };

  for (int i = 0; i < n; ++i) {
    swim(i);
  }
  for (int i = n - 1; i > 0; --i) {
    swap(vec[i], vec[0]);
    sink(0, i);
  }
}

// template class BubbleSort<int>;
// template class InsertSort<int>;
// template class MergeSort<int>;
// template class QuickSort<int>;