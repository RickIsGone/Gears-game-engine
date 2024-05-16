cmake_minimum_required(VERSION 3.28.0)
project(engine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

file(GLOB SOURCES "engine/*.cpp")
add_executable(engine ${SOURCES} ) 

# compiling shaders
execute_process(
    COMMAND compile.bat
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/engine"
    RESULT_VARIABLE batch_result
)

if(batch_result)
    message(FATAL_ERROR "L'esecuzione del batch file 'compile.bat' ha fallito.")
else()
    message("Il batch file 'compile.bat' è stato eseguito con successo.")
endif()


if(CMAKE_BUILD_TYPE MATCHES Release)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders)
    file(COPY engine/shaders/simple_shader.frag.spv DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders)
    file(COPY engine/shaders/simple_shader.vert.spv DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders)
    if(MSVC)
        set_target_properties(engine PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    else()
        set_target_properties(engine PROPERTIES LINK_FLAGS "-mwindows")
    endif()
else()
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders)
    file(COPY engine/shaders/simple_shader.frag.spv DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders)
    file(COPY engine/shaders/simple_shader.vert.spv DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders)
endif()


find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanUtilityLibraries REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(engine PRIVATE Vulkan::Vulkan glfw glm::glm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)