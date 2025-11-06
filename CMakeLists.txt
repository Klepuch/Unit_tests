cmake_minimum_required(VERSION 3.10)
project(OrderSystemTest)

find_package(GTest REQUIRED)

include_directories(include)

add_library(order_lib src/order.cpp include/order.h)
target_include_directories(order_lib PUBLIC include)

add_executable(order_test test/order_test.cpp)

target_link_libraries(order_test 
    order_lib 
    GTest::gtest_main 
    GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(order_test)
