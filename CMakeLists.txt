cmake_minimum_required(VERSION 3.16)
project(TreeSetLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(lab1_ui main.cpp)

add_executable(benchmark_runner benchmark/benchmark_runner.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_tests
    tests/test_containers.cpp
    tests/test_student.cpp
    tests/test_bst.cpp
    tests/test_set.cpp
)

target_link_libraries(run_tests
    GTest::gtest
    GTest::gtest_main
    pthread
)

enable_testing()
add_test(NAME AllModularTests COMMAND run_tests)
