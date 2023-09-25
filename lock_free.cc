#include <atomic>
#include <iostream>
#include <utility>

struct A {
  int a[100];
};

struct B {
  int x, y;
};

int main() {
  std::cout << std::boolalpha << "std::atomic<A> is lock free? "
            << std::atomic<A>::is_always_lock_free << '\n'
            << "std::atomic<B> is lock free? "
            << std::atomic<B>::is_always_lock_free << '\n'
            << "std::atomic<uint32_t> is lock free? "
            << std::atomic<uint32_t>::is_always_lock_free << '\n';
}