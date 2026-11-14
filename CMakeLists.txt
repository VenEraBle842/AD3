cmake_minimum_required(VERSION 3.16)

project(TreeSetLab LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(TREESETLAB_IS_TOP_LEVEL ON)
else()
    set(TREESETLAB_IS_TOP_LEVEL OFF)
endif()

option(
    TREESETLAB_BUILD_APP
    "Build the TreeSetLab console application"
    ${TREESETLAB_IS_TOP_LEVEL}
)

option(
    TREESETLAB_BUILD_TESTS
    "Build the TreeSetLab tests"
    ${TREESETLAB_IS_TOP_LEVEL}
)

option(
    TREESETLAB_BUILD_BENCHMARK
    "Build the TreeSetLab benchmark"
    ${TREESETLAB_IS_TOP_LEVEL}
)

add_library(treesetlab_core INTERFACE)
add_library(TreeSetLab::Core ALIAS treesetlab_core)

target_include_directories(
    treesetlab_core
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_features(
    treesetlab_core
    INTERFACE
        cxx_std_17
)

if(TREESETLAB_BUILD_APP)
    add_executable(
        lab1_ui
        main.cpp
    )

    target_link_libraries(
        lab1_ui
        PRIVATE
            TreeSetLab::Core
    )
endif()

if(TREESETLAB_BUILD_BENCHMARK)
    add_executable(
        benchmark_runner
        benchmark/benchmark_runner.cpp
    )

    target_link_libraries(
        benchmark_runner
        PRIVATE
            TreeSetLab::Core
    )
endif()

if(TREESETLAB_BUILD_TESTS)
    enable_testing()

    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)

    add_executable(
        run_tests
        tests/test_containers.cpp
        tests/test_student.cpp
        tests/test_bst.cpp
        tests/test_set.cpp
    )

    target_link_libraries(
        run_tests
        PRIVATE
            TreeSetLab::Core
            GTest::gtest_main
            Threads::Threads
    )

    add_test(
        NAME AllModularTests
        COMMAND run_tests
    )
endif()
