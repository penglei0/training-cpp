#include <cstdint>
#include <cstdio>
#include <typeinfo>

int main() {
    // int promotion & UB in shift
    uint8_t test = 0x80;
    auto value = (test >> 1);
    std::printf("value: 0x%02x, type: %s\n", value, typeid(value).name());

    // access unaligned data
    uint8_t buf[8];
    uint8_t *data = buf + 1;   // very likely not 4-byte aligned
    *((uint32_t *)data) = htonl(0x12345678);
}

// @ int promotion
// https://timsong-cpp.github.io/cppwp/conv.prom

// @ Undefined behavior on 'a << b' and 'b >> a'
// https://en.cppreference.com/w/cpp/language/operator_arithmetic.html

// @ access unaligned data
// https://mypages.iar.com/s/article/Accessing-Unaligned-Data?language=en_US
// onverting a pointer from a type to another without respecting this alignement rule is undefined behavior (http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf page 74, 7).
