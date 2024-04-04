cmake_minimum_required(VERSION 3.27)
project(GroceryMadeEasy)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(GROCERYMADEEASY_SOURCES main.cpp)
add_executable(GroceryMadeEasy main.cpp)

target_include_directories(GroceryMadeEasy PRIVATE /Users/saad/Library/CloudStorage/OneDrive-InstituteofBusinessAdministration/Intro\ to\ Programming\ IBA/raylib/raylib/src)
target_link_libraries(GroceryMadeEasy raylib)
