cmake_minimum_required(VERSION 3.5.0)
project(imgui VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

file(GLOB HEADERS "imgui/*.h")
file(GLOB SOURCE "imgui/*.cpp")

add_library(imgui STATIC   ${SOURCE}
                           ${HEADERS}
            )

target_include_directories(imgui PUBLIC imgui)

