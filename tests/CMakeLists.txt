cmake_minimum_required(VERSION 3.2)

project(tests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../mathcore")

add_executable(tests
    tst_triangle.cpp

    # import core sources
    ../mathcore/triangle.cpp
    ../mathcore/BaseFigure.cpp
)

add_test(NAME test_triangle COMMAND test_triangle)

target_link_libraries(tests PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)

