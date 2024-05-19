cmake_minimum_required(VERSION 3.28.0)
project(Gears VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

file(GLOB SOURCES "engine/*.cpp")
add_executable(Gears ${SOURCES} ) 


# compiling shaders
file(GLOB VERT_SHADERS "engine/shaders/*.vert")
file(GLOB FRAG_SHADERS "engine/shaders/*.frag")
list(APPEND SHADERS ${VERT_SHADERS} ${FRAG_SHADERS})

find_program(GLSL_EXECUTABLE NAMES glslc DOC "Path to glslc compiler")
if(NOT GLSL_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
endif()

#message("${SHADERS}")  #debug shaders
foreach(shader IN LISTS SHADERS)
    execute_process(
        COMMAND ${GLSL_EXECUTABLE} ${shader} -o ${shader}.spv
        RESULT_VARIABLE result_vert
    )
    if(result_vert)
        message(FATAL_ERROR "Failed to compile shader: ${shader}")
    endif()
endforeach(shader)


#coping compiled shaders and removing the console if the application is in release mode
if(CMAKE_BUILD_TYPE MATCHES Release)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders)
    foreach(shader IN LISTS SHADERS) 
        file(COPY ${shader}.spv DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders)
    endforeach(shader)
    if(MSVC)
        set_target_properties(Gears PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    else()
        set_target_properties(Gears PROPERTIES LINK_FLAGS "-mwindows")
    endif()
else()
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders)
    foreach(shader IN LISTS SHADERS)
        file(COPY ${shader}.spv DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders)
    endforeach(shader)
endif()

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(Gears PRIVATE Vulkan::Vulkan glfw glm::glm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)