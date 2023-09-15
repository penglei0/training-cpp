// hello world
//  Created by yym on 2020/10/13.
#include <benchmark/benchmark.h>
#include <string.h>

#include <iostream>
#include <memory>
#include <set>
#include <string>

class base {
 public:
  base() = default;
  virtual ~base() = default;
  virtual int calculate(int a, int b) const = 0;
};
using base_ptr = std::shared_ptr<base>;

class derived : public base {
 public:
  derived() = default;
  ~derived() override = default;
  int calculate(int a, int b) const override { return a + b; }
  int calculate2(int a, int b) const { return a + b; }
};

static void BM_virtual_function(benchmark::State& state) {
  base_ptr obj = std::make_shared<derived>();
  for (auto _ : state) {
    benchmark::DoNotOptimize(obj->calculate(1, 9));
  }
}

static void BM_dynamics_cast(benchmark::State& state) {
  base_ptr obj = std::make_shared<derived>();
  for (auto _ : state) {
    auto derived_obj = std::dynamic_pointer_cast<derived>(obj);
    benchmark::DoNotOptimize(derived_obj->calculate2(1, 9));
  }
}

BENCHMARK(BM_virtual_function);
BENCHMARK(BM_dynamics_cast);

BENCHMARK_MAIN();

/* Benchmark result:
```text
--------------------------------------------------------------
Benchmark                    Time             CPU   Iterations
--------------------------------------------------------------
BM_virtual_function      0.214 ns        0.213 ns   1000000000
BM_dynamics_cast          4.02 ns         4.02 ns    174115345
```
*/
