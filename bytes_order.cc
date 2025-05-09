#include <iostream>

// one way to avoid use ntohs, htons functions.
struct my_header {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  uint16_t a : 8;  // lower 8 bits
  uint16_t b : 8;  // higher 8 bits
#elif __BYTE_ORDER == __BIG_ENDIAN
  uint16_t b : 8;  // lower 8 bits
  uint16_t a : 8;  // higher 8 bits
#endif
};

// bit order usually follows the same endianness as the byte order for a given
// system.
// my_header2 can't be used to transfer data over the network.
struct my_header2 {
  uint8_t a : 1;
  uint8_t b : 2;
  uint8_t c : 5;
};

int main() {
  struct my_header header;
  *((uint16_t *)&header) = 0x2211;
  std::cout << std::hex << header.a << " " << header.b << std::endl;
  return 0;
}