cmake_minimum_required(VERSION 3.23.0)
project(Gears VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

if(NOT MSVC_IDE)
    # setting the build type to Release as a fallback when no type is provided during generation
    if(CMAKE_BUILD_TYPE STREQUAL "" OR NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE})
endif()

file(GLOB SOURCES "engine/*.cpp")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -Zi") # debug

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
message("Copying shaders in ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders)

foreach(shader IN LISTS SHADERS)
    file(COPY ${shader}.spv DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders)
endforeach(shader)

if(${CMAKE_SYSTEM} MATCHES Windows)
    if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
        add_executable(Gears WIN32 ${SOURCES} script.rc)
    else()
        add_executable(Gears ${SOURCES} script.rc)
        add_definitions(-DGRS_DEBUG)
    endif()
else()
    add_executable(Gears ${SOURCES})
endif()

# adding extern libs
add_subdirectory(engine/vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor/glm EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor/tinyobjloader EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor EXCLUDE_FROM_ALL) # imgui

find_package(Vulkan REQUIRED)

target_link_directories(Gears PRIVATE glfw glm::glm tinyobjloader imgui)
target_link_libraries(Gears PRIVATE Vulkan::Vulkan glfw glm::glm tinyobjloader imgui)
