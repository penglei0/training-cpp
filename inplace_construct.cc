#include <iostream>
#include <vector>
/// @brief concrete class
class Obj {
 public:
  Obj(int a, int b) { std::cout << "Obj()" << std::endl; }
  ~Obj() { std::cout << "~Obj()" << std::endl; }
  // copy constructor
  Obj(const Obj& rhs) { std::cout << "copy constructor" << std::endl; }
  // copy assignment operator
  Obj& operator=(const Obj& rhs) {
    std::cout << "copy assignment operator" << std::endl;
    return *this;
  }
  // move constructor
  Obj(Obj&& rhs) {
    std::cout << "move constructor" << std::endl;
    *this = std::move(rhs);
  }
  // move assignment operator
  Obj& operator=(Obj&& rhs) {
    std::cout << "move assignment operator" << std::endl;
    return *this;
  }
};

int main() {
  {
    std::vector<Obj> v;
    v.push_back(Obj(0, 1));  // move constructor
  }
  std::cout << "----------------" << std::endl;
  {
    std::vector<Obj> v;
    Obj o(1, 2);
    v.emplace_back(o);  // copy
  }

  std::cout << "----------------" << std::endl;
  {
    std::vector<Obj> v;
    v.emplace_back(1, 2);  // construct in place
  }
  return 0;
}
// construct the element in-place doesn't take effect.
