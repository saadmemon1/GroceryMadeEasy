cmake_minimum_required(VERSION 3.27)
project(GroceryMadeEasy)

set(CMAKE_CXX_STANDARD 14)

set(GROCERYMADEEASY_SOURCES main.cpp)
add_executable(GroceryMadeEasy ${GROCERYMADEEASY_SOURCES})
set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/raylib/4.5.0")

target_include_directories(GroceryMadeEasy PRIVATE "${RAYLIB_DIR}/include")
target_link_directories(GroceryMadeEasy PRIVATE "${RAYLIB_DIR}/lib")

find_library(COREFOUNDATION_LIBRARY CoreFoundation)
if(NOT COREFOUNDATION_LIBRARY)
    message(FATAL_ERROR "CoreFoundation not found")
endif()

target_link_libraries(GroceryMadeEasy ${COREFOUNDATION_LIBRARY} raylib)
