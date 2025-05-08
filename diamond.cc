/**
 * @file diamond.cc
 * @author Lei Peng (peng.lei@n-hop.com)
 * @brief None regular diamond inheritance
 * @version 3.6.0
 * @date 2025-05-08
 *
 *
 */
#include <iostream>

class interface {
 public:
  virtual ~interface() = default;
  virtual void name() = 0;
  virtual void test() = 0;
};

class interface2 : virtual public interface {
 public:
  virtual void name2() = 0;
  virtual void test2() = 0;
};

class implement : virtual public interface {
 public:
  void name() override { std::cout << "implement::name()" << std::endl; }
  void test() override { std::cout << "implement::test()" << std::endl; }
};

class implement2 : public implement, public interface2 {
 public:
  void name2() override { std::cout << "implement2::name2()" << std::endl; }
  void test2() override { std::cout << "implement2::test2()" << std::endl; }
};

int main() {
  implement2* obj = new implement2();
  obj->name();
  obj->test();
  obj->name2();
  obj->test2();

  interface* inf = obj;
  interface2* inf2 = obj;
  inf->name();
  inf->test();
  inf2->name2();
  inf2->test2();

  delete obj;

  return 0;
}