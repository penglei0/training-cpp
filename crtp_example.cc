#include <iostream>
#include <memory>

enum class TestType { kTest1, kTest2 };

template <class Derived>
class Base {
 public:
  void name() { (static_cast<Derived*>(this))->impl(); }
  // template function in CRTP
  template <TestType type>
  void test() {
    std::cout << "Base::test()" << std::endl;
    // example of how to call Derived::impl_test<type>()
    static_cast<Derived*>(this)->template impl_test<type>();
  }
};

class D1 : public Base<D1> {
 public:
  void impl() { std::puts("D1::impl()"); }
  template <TestType type>
  void impl_test() {
    if constexpr (type == TestType::kTest1) {
      std::cout << "impl kTest1" << std::endl;
    } else if constexpr (type == TestType::kTest2) {
      std::cout << "impl kTest2" << std::endl;
    }
  }
};
class D2 : public Base<D2> {
 public:
  void impl() { std::puts("D2::impl()"); }
};
// reference version
template <class Derived>
void handle(Base<Derived>& dd) {
  dd.name();
}

// pointer version
template <typename Derived>
void handle2(std::shared_ptr<Base<Derived>>& dd) {
  dd->name();
}

// using example
void test() {
  // Base<D1> b1; b1.name(); //undefined behavior
  // Base<D2> b2; b2.name(); //undefined behavior
  D1 d1;
  d1.name();
  D2 d2;
  d2.name();

  handle(d1);

  std::shared_ptr<Base<D1>> d11 = std::make_shared<D1>();
  std::shared_ptr<Base<D2>> d22 = std::make_shared<D2>();
  handle2(d11);
  handle2(d22);

  d11->test<TestType::kTest1>();
}

int main() { test(); }
