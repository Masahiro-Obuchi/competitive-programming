cmake_minimum_required(VERSION 3.22)

project(CompetitiveProgramming VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O2 -D_GLIB_CXX_DEBUG -g -fsanitize=undefined")

find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)

set(AC_LIBRARY_INCLUDE_DIR "~/cp_ws/Library/ac-library/")
set(BITS_STDCPP_INCLUDE_DIR "~/cp_ws/Library/")

add_executable(solution solution.cpp)
target_include_directories(solution PRIVATE ${Boost_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${AC_LIBRARY_INCLUDE_DIR} ${BITS_STDCPP_INCLUDE_DIR})
