add_executable(tests
  main.cpp
  dummy.cpp
)

target_link_libraries(tests
  lib
  gtest
)

target_include_directories(tests
  SYSTEM PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../googletest/googlemock/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../googletest/googletest/include
)
