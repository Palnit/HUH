set(TEST_SOURCES
        src/array_test.cpp
)

add_executable(Core_module_Tests ${TEST_SOURCES})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(
        Core_module_Tests
        HUH::Core
        GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(Core_module_Tests)

