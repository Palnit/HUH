find_package(benchmark REQUIRED)

add_executable(Core_array_benchmark src/Core_array_benchmark.cpp)
huh_target_compile_options(Core_array_benchmark PRIVATE GCClang -O3 MSVC /O2)

target_link_libraries(Core_array_benchmark HUH::Core benchmark::benchmark)
