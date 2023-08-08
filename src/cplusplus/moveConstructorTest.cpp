#include "gtest/gtest.h"
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

// 拷贝构造函数和移动构造函数
class String {
public:
  String(int size) : size_(size) { data_ = new char[size]; }
  String(const String &s) {
    size_ = s.size_;
    data_ = new char[size_];
    strcpy(data_, s.data_);
  }
  String(String &&s) : size_(s.size_), data_(s.data_) {
    s.size_ = 0;
    s.data_ = nullptr;
  }
  ~String() {
    if (data_ != nullptr) {
      delete[] data_;
    }
  }
  String *operator=(const String &s) {
    if (data_) {
      delete[] data_;
    }
    size_ = s.size_;
    data_ = new char[size_];
    strcpy(data_, s.data_);
  }
  String *operator=(String &&s) {
    if (data_) {
      delete[] data_;
    }
    size_ = s.size_;
    data_ = s.data_;
    s.size_ = 0;
    s.data_ = nullptr;
  }

private:
  int size_;
  char *data_;
};