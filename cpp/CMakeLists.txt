cmake_minimum_required(VERSION 3.14)
project(smart_food_manager VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build the test suite" ON)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

# Find required packages
find_package(Threads REQUIRED)
if(BUILD_PYTHON_BINDINGS)
    find_package(pybind11 REQUIRED)
endif()

# Set source files
set(SOURCES
    src/core/meal.cpp
    src/core/recipe.cpp
    src/core/ingredient.cpp
    src/core/storage.cpp
)

# Set header files
set(HEADERS
    include/smart_food/core/meal.hpp
    include/smart_food/core/recipe.hpp
    include/smart_food/core/ingredient.hpp
    include/smart_food/core/storage.hpp
)

# Create library
add_library(smart_food SHARED ${SOURCES} ${HEADERS})
target_include_directories(smart_food
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(smart_food PRIVATE Threads::Threads)

# Compiler warnings
if(MSVC)
    target_compile_options(smart_food PRIVATE /W4)
else()
    target_compile_options(smart_food PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Testing
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Python bindings
if(BUILD_PYTHON_BINDINGS)
    add_subdirectory(bindings)
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS smart_food
    EXPORT smart_food-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
