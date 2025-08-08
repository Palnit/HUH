set(TEST_SOURCES
        src/vector_test.cpp
)

add_executable(Math_module_Tests ${TEST_SOURCES})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(
        Math_module_Tests
        HUH::Math
        GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(Math_module_Tests)

