cmake_minimum_required(VERSION 3.5)

project(figen-poc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

include_directories(inc/)
set(PROJECT_SOURCES
        src/main.cpp
        src/window.cpp inc/window.hpp
        inc/canvas.hpp src/canvas.cpp
        inc/NGonFigure.hpp src/NGonFigure.cpp
        resource.qrc
)

if(MSVC)
    add_compile_options(/W3)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(figen-poc
        WIN32 # Turn off console on Windows
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET figen-poc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(figen-poc SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(figen-poc
            WIN32 # Turn off console on Windows
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(
    clang-tidy-check clang-tidy -p ${CMAKE_BINARY_DIR}/compile_commands.json -checks=cert* ${sources}
    DEPENDS ${sources}
)
add_custom_target(check DEPENDS clang-tidy-check)

add_subdirectory("mathcore/")

target_link_libraries(figen-poc PRIVATE mathcore Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(figen-poc PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER squares.gogacoder.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(figen-poc)
endif()

