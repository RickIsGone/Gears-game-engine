cmake_minimum_required(VERSION 3.28.0)
project(engine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

file(GLOB SOURCES "*.cpp")
add_executable(engine ${SOURCES} ) 


if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        set_target_properties(engine PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    else()
        set_target_properties(engine PROPERTIES LINK_FLAGS "-mwindows")
    endif()
endif()


find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanUtilityLibraries REQUIRED)

#target_link_libraries(main PRIVATE Vulkan::LayerSettings Vulkan::UtilityHeaders Vulkan::CompilerConfiguration)
target_link_libraries(engine PRIVATE Vulkan::Vulkan glfw Vulkan::UtilityHeaders)

file(COPY shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)