#include <random>
#include <thread>
#include <boost/core/ignore_unused.hpp>
#include <benchmark/benchmark.h>
#include "lib.hpp"

namespace {
  std::default_random_engine random;
  std::string generate_random_string(const std::size_t size) {
    constexpr std::size_t range = 'z' - 'a';
    std::string ret;

    ret.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
      char c = 'a' + (random()%range);
      ret.push_back(c);
    }

    return ret;
  }

  std::string generate_random_palindrome(const std::size_t size) {
    std::string palin = generate_random_string(size);
    palin.reserve(size*2);
    std::reverse_copy(palin.begin(), palin.end(), std::back_inserter(palin));
    return palin;
  }
}

static void benchmark_serial(benchmark::State& state) {
  const auto string_size = state.range(0);
  for (auto _ : state) {
    state.PauseTiming();
    const std::string str = generate_random_palindrome(string_size);
    state.ResumeTiming();
    benchmark::DoNotOptimize(is_palindrome_permutation_array(str));
  }
  state.SetComplexityN(state.range(0) * state.iterations());
  state.SetBytesProcessed(string_size * state.iterations() * 2);
  state.counters["verified"]=true;
}
//BENCHMARK(benchmark_serial)
//->RangeMultiplier(2)->Range(1,1000000)->Complexity(benchmark::oN)->UseRealTime();

static void benchmark_parallel(benchmark::State& state) {
  const auto string_size = state.range(0);
  const auto threads = state.range(1);
  for (auto _ : state) {
    state.PauseTiming();
    const std::string str = generate_random_palindrome(string_size);
    state.ResumeTiming();
    benchmark::DoNotOptimize(is_palindrome_permutation_parallel(str, threads));
  }
  state.SetComplexityN(state.range(0) * state.iterations());
  state.SetBytesProcessed(string_size * state.iterations() * 2);
  state.counters["verified"]=true;
}
static void CustomArguments(benchmark::internal::Benchmark* b) {
  for(int64_t i=10; i<100000000; i = i<<1) {
    for(int64_t th=1; th<=12; ++th) {
      b->Args({i, th});
    }
  }
}
BENCHMARK(benchmark_parallel)
->Apply(CustomArguments)
->Complexity(benchmark::oN)->UseRealTime();

BENCHMARK_MAIN();
