cmake_minimum_required(VERSION 3.28.0)
project(Gears-engine VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT MSVC_IDE)
    # setting the build type to Release as a fallback when no type is provided during generation
    if(CMAKE_BUILD_TYPE STREQUAL "" OR NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BUILD_TYPE} ${CMAKE_SYSTEM_NAME}")
endif()

file(GLOB SOURCES "engine/*.cpp")

# debug
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zi")
    endif()
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
        add_executable(Gears WIN32 ${SOURCES} script.rc)
    else()
        add_executable(Gears ${SOURCES} script.rc)
        add_definitions(-DGRS_DEBUG)
    endif()
else()
    add_executable(Gears ${SOURCES})
    if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
        add_definitions(-DGRS_DEBUG)
    endif()
endif()

#adding modules
file(GLOB MODULES "engine/*.cppm")
target_sources(Gears PUBLIC FILE_SET gears_modules TYPE CXX_MODULES FILES
   ${MODULES}
)

# adding extern libs
add_subdirectory(engine/vendor/glfw EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor/glm EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor/tinyobjloader EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor/stb_image EXCLUDE_FROM_ALL)
add_subdirectory(engine/vendor EXCLUDE_FROM_ALL) # imgui

find_package(Vulkan REQUIRED)

target_link_directories(Gears PRIVATE glfw glm::glm tinyobjloader imgui stb_image)
target_link_libraries(Gears PRIVATE Vulkan::Vulkan glfw glm::glm tinyobjloader imgui stb_image)


# compiling shaders
file(GLOB VERT_SHADERS "engine/shaders/*.vert")
file(GLOB FRAG_SHADERS "engine/shaders/*.frag")
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
file(COPY media/gears_default_icon.png DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/icons)
file(COPY media/gears-w.png DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/icons)

# cpack
install(DIRECTORY "build/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/" DESTINATION .)
if(${CMAKE_SYSTEM} MATCHES Windows)
    set(CPACK_GENERATOR "NSIS;ZIP;7Z;WIX")
endif()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_NAME "Gears game engine")
include(CPack)
