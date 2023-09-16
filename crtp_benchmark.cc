#include <benchmark/benchmark.h>

#include <iostream>
#include <memory>
#include <variant>

template <class Derived>
class CrtpBase {
 public:
  int calcu(int a, int b) {
    return static_cast<Derived*>(this)->template impl_calcu(a, b);
  }
  int calcu1(int a, int b) {
    return static_cast<Derived*>(this)->template impl_calcu1(a, b);
  }
  int calcu2(int a, int b) {
    return static_cast<Derived*>(this)->template impl_calcu2(a, b);
  }
};

class CrtpDerived : public CrtpBase<CrtpDerived> {
 public:
  int impl_calcu(int a, int b) { return a + b; }
  int impl_calcu1(int a, int b) { return a + b; }
  int impl_calcu2(int a, int b) { return a + b; }
};

class BaseInterface {
 public:
  virtual int calcu(int a, int b) = 0;
  virtual int calcu1(int a, int b) = 0;
  virtual int calcu2(int a, int b) = 0;
};
using base_ptr = std::shared_ptr<BaseInterface>;

class Derived : public BaseInterface {
 public:
  int calcu(int a, int b) override { return a + b; }
  int calcu1(int a, int b) override { return a + b; }
  int calcu2(int a, int b) override { return a + b; }
};

static void BM_virtual_function(benchmark::State& state) {
  base_ptr obj = std::make_shared<Derived>();
  for (auto _ : state) {
    benchmark::DoNotOptimize(obj->calcu(1, 9));
    benchmark::DoNotOptimize(obj->calcu1(1, 9));
    benchmark::DoNotOptimize(obj->calcu2(1, 9));
  }
}

static void BM_crtp_pattern(benchmark::State& state) {
  // std::shared_ptr<CrtpBase<CrtpDerived>> obj = ...
  std::variant<std::shared_ptr<CrtpBase<CrtpDerived>>> obj_var =
      std::make_shared<CrtpDerived>();
  for (auto _ : state) {
    int res = 0;
    benchmark::DoNotOptimize(
        res = std::visit([](auto&& arg) -> int { return arg->calcu(1, 9); },
                         obj_var));
    benchmark::DoNotOptimize(
        res = std::visit([](auto&& arg) -> int { return arg->calcu1(1, 9); },
                         obj_var));
    benchmark::DoNotOptimize(
        res = std::visit([](auto&& arg) -> int { return arg->calcu2(1, 9); },
                         obj_var));
  }
}

BENCHMARK(BM_virtual_function);
BENCHMARK(BM_crtp_pattern);

BENCHMARK_MAIN();

/* Benchmark result:
```text
2023-09-16T02:30:24+00:00
Running ./crtp_benchmark
Run on (2 X 3176.12 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x1)
  L1 Instruction 32 KiB (x1)
  L2 Unified 512 KiB (x1)
  L3 Unified 32768 KiB (x1)
Load Average: 0.30, 0.25, 0.25
--------------------------------------------------------------
Benchmark                    Time             CPU   Iterations
--------------------------------------------------------------
BM_virtual_function       1.12 ns         1.11 ns    652243999
BM_crtp_pattern          0.313 ns        0.313 ns   1000000000
```
*/