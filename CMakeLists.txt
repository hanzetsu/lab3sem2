cmake_minimum_required(VERSION 3.16)
project(Lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OLD_INCLUDE_PATH "C:/Users/eduar/OneDrive/Desktop/c++/lab2sem2/include")
set(OLD_TESTS_PATH   "C:/Users/eduar/OneDrive/Desktop/c++/lab2sem2/tests")

add_executable(Lab3 source/main.cpp)
target_include_directories(Lab3 PRIVATE 
    ${OLD_INCLUDE_PATH} 
    ${OLD_TESTS_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_matrix tests/test_matrix.cpp)
target_include_directories(test_matrix PRIVATE
    ${OLD_INCLUDE_PATH}
    ${OLD_TESTS_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME MatrixTest COMMAND test_matrix)