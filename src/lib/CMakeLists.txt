add_library(lib STATIC
  src/lib.cpp

  include/lib.hpp
)

target_compile_definitions(lib PUBLIC
  BOOST_ERROR_CODE_HEADER_ONLY
)

target_include_directories(lib
  SYSTEM PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../boost
)

target_include_directories(lib
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
