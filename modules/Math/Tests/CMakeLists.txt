set(TEST_SOURCES
        src/vector_test.cpp
)

add_executable(Math_Module_Tests ${TEST_SOURCES})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(
        Math_Module_Tests
        HUH::Math
        GTest::gtest_main
)

#add_test(Math_Module_Tests)

include(GoogleTest)
gtest_discover_tests(Math_Module_Tests)

