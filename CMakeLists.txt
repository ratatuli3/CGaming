cmake_minimum_required(VERSION 3.10.0)
project(CGaming)

add_executable(${PROJECT_NAME} main.c)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find SDL2_Image
find_package(SDL2_image REQUIRED)

# Link the SDL2 library to your project
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_image::SDL2_image)
