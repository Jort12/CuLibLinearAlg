cmake_minimum_required(VERSION 3.18)
project(CuLibLinearAlg LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES 89)

find_package(CUDAToolkit REQUIRED)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(linalg STATIC
    src/matrix.cpp
    src/ops_cpu.cpp
)

target_include_directories(linalg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(linalg PRIVATE /W4)
else()
    target_compile_options(linalg PRIVATE -Wall -Wextra)
endif()

add_executable(linalg_tests tests/test_main.cpp)
target_link_libraries(linalg_tests PRIVATE linalg)

add_executable(linalg_bench benchmarks/bench_main.cpp)
target_link_libraries(linalg_bench PRIVATE linalg)

add_executable(linalg_example examples/example_basic.cpp)
target_link_libraries(linalg_example PRIVATE linalg)

foreach(target linalg_tests linalg_bench linalg_example)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
endforeach()

enable_testing()
add_test(NAME linalg_tests COMMAND linalg_tests)
