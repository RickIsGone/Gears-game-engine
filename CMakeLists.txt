cmake_minimum_required(VERSION 3.23.0)
project(Gears VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE D:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake)
include(CTest)
enable_testing()

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(BUILD_TYPE "Release")
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D DEBINFO)
    set(BUILD_TYPE "Debug")
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    set(BUILD_TYPE "MinSizeRel")
else()
    set(BUILD_TYPE "RelWithDebInfo")
    add_definitions(-D DEBINFO)
endif()


file(GLOB SOURCES "engine/*.cpp")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -Zi")    #debug


# compiling shaders
file(GLOB VERT_SHADERS "engine/shaders/*.vert")
file(GLOB FRAG_SHADERS "engine/shaders/*.frag")
list(APPEND SHADERS ${VERT_SHADERS} ${FRAG_SHADERS})

find_program(GLSL_EXECUTABLE NAMES glslc DOC "Path to glslc compiler")
if(NOT GLSL_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
endif()

foreach(shader IN LISTS SHADERS)
    execute_process(
        COMMAND ${GLSL_EXECUTABLE} ${shader} -o ${shader}.spv
        RESULT_VARIABLE result_vert
    )
    if(result_vert)
        message(FATAL_ERROR "Failed to compile shader: ${shader}")
    endif()
endforeach(shader)


# copying compiled shaders and removing the console if the application is in release mode
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${BUILD_TYPE}/shaders)
foreach(shader IN LISTS SHADERS) 
    file(COPY ${shader}.spv DESTINATION ${CMAKE_BINARY_DIR}/${BUILD_TYPE}/shaders)
endforeach(shader)
if(${CMAKE_SYSTEM} MATCHES Windows)
    if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
        add_executable(Gears WIN32 ${SOURCES}) 
    else()
        add_executable(Gears ${SOURCES})
    endif()
else()
    add_executable(Gears ${SOURCES})
endif()


# adding extern libs 
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(tinyobjloader REQUIRED)

target_link_libraries(Gears PRIVATE Vulkan::Vulkan glfw glm::glm tinyobjloader::tinyobjloader)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
