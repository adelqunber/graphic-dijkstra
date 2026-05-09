cmake_minimum_required(VERSION 3.10)
project(Dijkstra_Project C)

set(CMAKE_C_STANDARD 11)

# Raylib paths for your Ubuntu environment
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# We will create one clean executable for the GUI animation
add_executable(ShortestPathApp GUI.c)

# Crucial: This links Raylib + all the Linux system libraries it depends on
target_link_libraries(ShortestPathApp raylib GL m pthread dl rt X11)