cmake_minimum_required(VERSION 3.10.0)
project(CGaming)

add_executable(${PROJECT_NAME} main.c status.c)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Link the SDL2 library to your project
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
