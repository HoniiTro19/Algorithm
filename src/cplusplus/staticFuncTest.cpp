#include <iostream>
#include <type_traits>

template<typename T, typename = void>
struct has_foo_check : std::false_type {};

// C++17
// template<typename T>
// struct has_foo_check<T, std::void_t<decltype(T::foo())>> : std::true_type {};

// C++11
template<typename T>
struct has_foo_check<T, decltype(T::foo(), void())> : std::true_type {};

template<typename T>
void call_has_foo_check() {
    static_assert(has_foo_check<T>::value, "do not have foo");
    T::foo();
}

struct HasFoo {
    static void foo() {
        std::cout << "has foo" << std::endl;
    }
};

struct NotHasFoo {};

int main() {
    call_has_foo_check<HasFoo>();
    //call_has_foo_check<NotHasFoo>();
    return 0;
}