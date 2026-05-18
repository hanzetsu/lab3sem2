cmake_minimum_required(VERSION 3.16)
project(Lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(OLD_INCLUDE_PATH "C:/Users/eduar/OneDrive/Desktop/c++/lab2sem2/include")
set(OLD_TESTS_PATH   "C:/Users/eduar/OneDrive/Desktop/c++/lab2sem2/tests")

set(GUI_SOURCES
    source/main.cpp
    source/MatrixCalculatorWidget.cpp
    source/MainWindow.cpp
    source/TrajectoryWidget.cpp
    source/TrajectoryCanvas.cpp
)

set(GUI_HEADERS
    include/MatrixCalculatorWidget.hpp
    include/MainWindow.hpp
    include/TrajectoryWidget.hpp
    include/TrajectoryCalculator.hpp
    include/Vector.hpp
    include/constants.hpp
    include/TrajectoryCanvas.hpp
)

qt_add_executable(Lab3 ${GUI_SOURCES} ${GUI_HEADERS})
target_include_directories(Lab3 PRIVATE 
    ${OLD_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Lab3 PRIVATE Qt6::Core Qt6::Widgets)


add_executable(test_matrix tests/test_matrix.cpp)
target_include_directories(test_matrix PRIVATE
    ${OLD_INCLUDE_PATH}
    ${OLD_TESTS_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
enable_testing()
add_test(NAME MatrixTest COMMAND test_matrix)

add_executable(test_vector tests/test_vector.cpp)
target_include_directories(test_vector PRIVATE
    ${OLD_INCLUDE_PATH}
    ${OLD_TESTS_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_test(NAME VectorTest COMMAND test_vector)

add_executable(test_trajectory tests/test_trajectory.cpp)
target_include_directories(test_trajectory PRIVATE
    ${OLD_INCLUDE_PATH}
    ${OLD_TESTS_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_test(NAME TrajectoryTest COMMAND test_trajectory)