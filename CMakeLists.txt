cmake_minimum_required(VERSION 3.26)
project(Kursach)

set(CMAKE_CXX_STANDARD 20)

add_executable(Kursach Metrics/main.cpp)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/gtest)
SET(GTEST gtest/gtest-all.cc gtest/gtest_main.cc gtest/old_version.cpp)
add_executable(Test ${GTEST} Tests/HandwrittenTests.cpp Tests/RetroactiveArray1_0_Tests.cpp)
