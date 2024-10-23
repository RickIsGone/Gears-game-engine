cmake_minimum_required(VERSION 3.25.0)
project(imgui VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

file(GLOB IMGUI_HEADERS "imgui/*.h")
file(GLOB IMGUI_SOURCE "imgui/*.cpp")

add_library(imgui STATIC   ${IMGUI_SOURCE}
                           ${IMGUI_HEADERS}
            )
target_include_directories(imgui PUBLIC imgui)


