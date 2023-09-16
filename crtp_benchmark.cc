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