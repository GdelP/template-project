cmake_minimum_required(VERSION 3.10)
# Configure C++ compiler (trust the one available in PATH)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-fstack-protector -fstack-protector-all -Wall")
project(template-project)

add_subdirectory(src)
