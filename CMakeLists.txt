cmake_minimum_required(VERSION 3.16)

project(untitled1)
set(GCC_COVERAGE_COMPILE_FLAGS "-pedantic -std=c++17 -pthread -Wall -Wextra -Wformat=2 -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wsign-conversion -Wstrict-overflow=2 -Wundef -Wnull-dereference -Wuninitialized -Walloca -Wcast-qual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_CXX_STANDARD 17)


add_executable(untitled1 sudoku/sudoku.cpp substring/substring.cpp sort.cpp)