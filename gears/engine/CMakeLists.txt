cmake_minimum_required(VERSION 3.28.0)
project(Gears-engine-library VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.cpp")
add_library(Gears-engine STATIC ${SOURCES})

#adding modules
file(GLOB MODULES "*.cppm")
target_sources(Gears-engine PUBLIC FILE_SET gears_modules TYPE CXX_MODULES FILES
   ${MODULES}
)

# adding extern libs
add_subdirectory(../vendor/glfw ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(../vendor/glm ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glm EXCLUDE_FROM_ALL)
add_subdirectory(../vendor/tinyobjloader ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/tinyobjloader EXCLUDE_FROM_ALL)
add_subdirectory(../vendor/stb_image ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/stb_image EXCLUDE_FROM_ALL)

find_package(Vulkan REQUIRED)

target_link_directories(Gears-engine PRIVATE glfw glm::glm tinyobjloader stb_image)
target_link_libraries(Gears-engine PRIVATE Vulkan::Vulkan glfw glm::glm tinyobjloader stb_image)


# compiling shaders
file(GLOB VERT_SHADERS "shaders/*.vert")
file(GLOB FRAG_SHADERS "shaders/*.frag")
list(APPEND SHADERS ${VERT_SHADERS} ${FRAG_SHADERS})

# find_program(GLSL_EXECUTABLE NAMES glslc DOC "Path to glslc compiler")

# if(NOT GLSL_EXECUTABLE)
#     message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
# endif()

# foreach(shader IN LISTS SHADERS)
#     execute_process(
#         COMMAND ${GLSL_EXECUTABLE} ${shader} -o ${shader}.spv
#         RESULT_VARIABLE result_vert
#     )

#     if(result_vert)
#         message(FATAL_ERROR "Failed to compile shader: ${shader}")
#     endif()
# endforeach(shader)

# copying compiled shaders and removing the console if the application is in release mode
message("Copying shaders in ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

foreach(shader IN LISTS SHADERS)
    file(COPY ${shader}.spv DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
endforeach(shader)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/icons)
file(COPY ../media/gears_default_icon.png DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/icons)
file(COPY ../media/gears-w.png DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/icons)

