cmake_minimum_required(VERSION 3.25.0)
project(stb_image VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_library(stb_image STATIC
            stb_image.cpp stb_image.h
            )
target_include_directories(stb_image PUBLIC stb_image)