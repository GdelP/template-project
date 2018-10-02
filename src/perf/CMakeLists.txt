add_executable(perf
  main.cpp
  )

target_link_libraries(perf
  lib
  benchmark)

target_include_directories(lib
  SYSTEM PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../boost
)
