#include <iostream>
#include <memory>
#include <variant>

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

  std::string& id() { return static_cast<Derived*>(this)->impl_id(); }
  const std::string& id() const {
    return static_cast<const Derived*>(this)->impl_id();
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
  std::string& impl_id() { return id_; }
  const std::string& impl_id() const { return id_; }

 private:
  std::string id_ = "D1";
};
class D2 : public Base<D2> {
 public:
  void impl() { std::puts("D2::impl()"); }
  std::string& impl_id() { return id_; }
  const std::string& impl_id() const { return id_; }

 private:
  std::string id_ = "D2";
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

  // variant
  std::variant<std::shared_ptr<Base<D1>>, std::shared_ptr<Base<D2>>> obj;
  obj = d11;
  obj = d22;
  std::cout << "============== variant ==============" << std::endl;
  std::visit([](auto&& arg) { arg->name(); }, obj);

  auto& id =
      std::visit([](auto&& arg) -> std::string& { return arg->id(); }, obj);
  std::cout << "id: " << id << std::endl;
}

int main() { test(); }
