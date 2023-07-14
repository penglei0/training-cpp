#include <stdint.h>

#include <functional>  // std::reference_wrapper
#include <iostream>
#include <string>
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

int main() {
  octetVec data;
  data.resize(10);

  // wrapper of data
  BufferProvider bufp;
  bufp.AddBuffer(data);
  auto& buf = bufp.GetBuffer(0);
  buf[0] = 99;

  std::cout << "data[0]: " << (int)data[0] << std::endl;
  return 0;
}