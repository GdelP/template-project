# Third party repositories
add_subdirectory(googletest)
# No need to (always) test third party libs
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)
#add_subdirectory(boost) # Use header-only libs for now

# lib contains the implementation and headers for our problem
add_subdirectory(lib)
# test contains correctness tests over lib
add_subdirectory(test)
# perf contains performance tests over lib
add_subdirectory(perf)
