find_package(benchmark REQUIRED)

add_executable(Math_module_benchmark src/Math_module_benchmarks.cpp)

target_link_libraries(Math_module_benchmark HUH::Math benchmark::benchmark)
