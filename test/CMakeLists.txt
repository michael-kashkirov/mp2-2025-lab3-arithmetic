set(TEST_SOURCES 
    test_stack.cpp
    test_arithmetic.cpp
    test_main.cpp
)

add_executable(gtest_arithmetic ${TEST_SOURCES})

target_include_directories(gtest_arithmetic PUBLIC
    ${gtest_SOURCE_DIR} 
)

target_link_libraries(gtest_arithmetic
    arithmetic
    gtest
)