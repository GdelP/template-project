#include <benchmark/benchmark.h>
#include <boost/core/ignore_unused.hpp>
#include "lib.hpp"

static void benchmark_my_func(benchmark::State& state) {
  for (auto _ : state) {
    auto first_param = state.range(0);
    boost::ignore_unused(first_param);
    benchmark::DoNotOptimize(my_func());
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(benchmark_my_func)->Range(1,10)->Complexity()->UseRealTime();

BENCHMARK_MAIN();
