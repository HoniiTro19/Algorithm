#include <iostream>
#include <atomic>

struct ControBlock {
    std::atomic<int64_t> ref_cnt;
    ControBlock() : ref_cnt(1) {}
    void add_ref() {
        ref_cnt.fetch_add(1, std::memory_order_relaxed);
    }
    int64_t sub_ref() {
        int64_t old = ref_cnt.fetch_sub(1, std::memory_order_acq_rel);
        return old - 1;
    }
};

template<typename T>
class SharedPtr {
private:
    T *ptr_;
    ControBlock *block_;
public:
    SharedPtr() : ptr_(nullptr), block_(nullptr) {}
    SharedPtr(T *value) :
        ptr_(value), block_(value ? new ControBlock() : nullptr) {}
    SharedPtr(const SharedPtr &other) :
        ptr_(other.ptr_), block_(other.block_) {
        if (block_) {
            block_->add_ref();
        }
    }
    SharedPtr(SharedPtr &&other) :
        ptr_(other.ptr_), block_(other.block_) {
        other.ptr_ = nullptr;
        other.block_ = nullptr;
    }
    ~SharedPtr() {
        if (block_ && block_->sub_ref() == 0) {
            delete ptr_;
            delete block_;
        }
        ptr_ = nullptr;
        block_ = nullptr;
    }

    SharedPtr &operator=(SharedPtr other) {
        std::swap(ptr_, other.ptr_);
        std::swap(block_, other.block_);
        return *this;
    }
    T *get_ptr() {
        return ptr_;
    }
};

struct TestClass {
    int value{0};
};

template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    TestClass *test_class = new TestClass();
    SharedPtr<TestClass> ptr0;
    SharedPtr<TestClass> ptr1(test_class);
    ptr0 = ptr1;
    ptr0.get_ptr()->value = 100;
    std::cout << ptr1.get_ptr()->value << std::endl;
    return 0;
}