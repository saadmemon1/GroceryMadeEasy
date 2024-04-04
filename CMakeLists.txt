cmake_minimum_required(VERSION 3.27)
project(GroceryMadeEasy)

set(CMAKE_CXX_STANDARD 14)

set(GROCERYMADEEASY_SOURCES main.cpp)
add_executable(GroceryMadeEasy main.cpp)

target_include_directories(GroceryMadeEasy PRIVATE /Users/saad/Library/CloudStorage/OneDrive-InstituteofBusinessAdministration/Intro to Programming IBA/raylib/raylib/src)
target_link_directories(GroceryMadeEasy PRIVATE /Users/saad/Library/CloudStorage/OneDrive-InstituteofBusinessAdministration/Intro to Programming IBA/raylib/raylib/src)
target_include_directories(GroceryMadeEasy PRIVATE /opt/homebrew/Cellar/raylib/4.5.0/include)
target_link_directories(GroceryMadeEasy PRIVATE /opt/homebrew/Cellar/raylib/4.5.0/lib)
target_link_libraries(GroceryMadeEasy raylib)
