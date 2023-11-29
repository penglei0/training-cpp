#include <stdint.h>

#include <functional>  // std::reference_wrapper
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using octetVec = std::vector<uint8_t>;

class BufferProvider {
 public:
  BufferProvider() {}
  virtual ~BufferProvider() {}
  octetVec& GetBuffer(int id) { return buf_list_[id]; }
  const octetVec& GetBuffer(int id) const { return buf_list_[id]; }
  void AddBuffer(octetVec& in) { buf_list_.push_back(in); }

 private:
  std::vector<std::reference_wrapper<octetVec>> buf_list_;
};

class Element {
 public:
  Element() {}
  virtual ~Element() {}
  // disable copy constructor
  Element(const Element&) = delete;
  // disable copy assignment operator
  Element& operator=(const Element&) = delete;
  // enable move constructor
  Element(Element&&) = delete;
  // enable move assignment operator
  Element& operator=(Element&&) = delete;
  void Print() const { std::cout << "Element::Print()" << std::endl; }
};

class Test {
 public:
  std::tuple<const Element&, const Element&> GetTuple() const {
    return std::make_tuple(std::ref(a), std::ref(b));
  }

 private:
  Element a;
  Element b;
};

int main() {
  std::unordered_map<int, std::reference_wrapper<octetVec>> my_map;
  octetVec data;
  data.resize(10);

  // wrapper of data
  BufferProvider bufp;
  bufp.AddBuffer(data);
  auto& buf = bufp.GetBuffer(0);
  buf[0] = 99;

  my_map.insert(std::make_pair(0, std::ref(data)));

  std::cout << "data[0]: " << (int)data[0] << std::endl;
  std::cout << "map data[0]: " << (int)my_map.at(0).get()[0] << std::endl;

  my_map.at(0).get()[0] = 100;

  std::cout << "data[0]: " << (int)data[0] << std::endl;

  Test tt;
  const auto& [a, b] = tt.GetTuple();
  a.Print();
  b.Print();
  return 0;
}