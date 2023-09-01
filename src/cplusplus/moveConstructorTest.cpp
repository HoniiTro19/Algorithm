#include "gtest/gtest.h"
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

// 带引用计数的String类型
class String {
public:
  String(int size) : size_(size) {
    cout << "default constructor" << endl;
    cnt_ = new int(1);
    data_ = new char[size];
  }
  String(String &&s) : size_(s.size_), cnt_(s.cnt_), data_(s.data_) {
    cout << "move constructor" << endl;
    s.size_ = 0;
    s.data_ = nullptr;
    s.cnt_ = nullptr;
  }
  String(const String &s) : size_(s.size_), cnt_(s.cnt_) {
    cout << "copy constructor" << endl;
    data_ = new char[size_];
    strcpy(data_, s.data_);
    ++*cnt_;
  }
  ~String() {
    if (*cnt_)
      --*cnt_;
    if (*cnt_ == 0 && data_ != nullptr) {
      cout << "count zero destructor" << endl;
      delete[] data_;
    }
  }
  String *operator=(String &&s) {
    if (data_) {
      delete[] data_;
    }
    size_ = s.size_;
    data_ = s.data_;
    s.size_ = 0;
    s.data_ = nullptr;
    s.cnt_ = nullptr;
    return this;
  }
  String *operator=(const String &s) {
    if (data_) {
      delete[] data_;
    }
    size_ = s.size_;
    data_ = new char[size_];
    strcpy(data_, s.data_);
    cnt_ = s.cnt_;
    ++*cnt_;
    return this;
  }
  int getCount() const { return cnt_ ? *cnt_ : 0; }

private:
  int size_;
  char *data_;
  int *cnt_;
};

int main() {
  String tmp(10);
  String cpy(tmp);
  cout << "tmp count: " << tmp.getCount() << endl;
  cout << "cpy count: " << cpy.getCount() << endl;
  return 0;
}