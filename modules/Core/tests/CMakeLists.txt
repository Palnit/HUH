set(TEST_SOURCES
        src/array_test.cpp
        src/pair_test.cpp
        src/tuple_test.cpp
)

add_executable(Core_module_Tests ${TEST_SOURCES})
huh_target_compile_options(Core_module_Tests PRIVATE GCClang -O0 MSVC /O0)
find_package(GTest CONFIG REQUIRED)

target_link_libraries(
        Core_module_Tests
        HUH::Core
        GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(Core_module_Tests)

