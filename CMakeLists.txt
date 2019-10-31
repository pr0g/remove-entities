cmake_minimum_required(VERSION 3.13)
project(remove-entities)
add_executable(${PROJECT_NAME} main.cpp)
find_package(benchmark)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)
target_compile_features(
    ${PROJECT_NAME} PRIVATE cxx_std_11)
target_link_libraries(
    ${PROJECT_NAME}
    benchmark::benchmark
    benchmark::benchmark_main
)
