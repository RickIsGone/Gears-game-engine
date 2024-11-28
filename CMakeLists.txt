# cmake_minimum_required(VERSION 3.28.0)
# project(Gears-engine VERSION 0.1.0 LANGUAGES C CXX)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)


# if(NOT MSVC_IDE)
#     # setting the build type to Release as a fallback when no type is provided during generation
#     if(CMAKE_BUILD_TYPE STREQUAL "" OR NOT DEFINED CMAKE_BUILD_TYPE)
#         set(CMAKE_BUILD_TYPE "Release")
#     endif()

#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BUILD_TYPE} ${CMAKE_SYSTEM_NAME}")
# endif()


# # debug
# if(CMAKE_BUILD_TYPE MATCHES Debug)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#     if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zi")
#     endif()
# endif()

# if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
#     add_executable(Gears ${SOURCES} script.rc)
# else()
#     add_executable(Gears ${SOURCES})
# endif()
# if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
#     add_definitions(-DGRS_DEBUG)
# endif()

# file(GLOB MODULES "gears/editor*.cppm")
# target_sources(Gears PUBLIC FILE_SET gears_modules TYPE CXX_MODULES FILES
#    ${MODULES}
# )

# calling the other cmake
# add_subdirectory(gears/editor EXCLUDE_FROM_ALL)

# target_link_directories(Gears PRIVATE Gears-engine )
# target_link_libraries(Gears PRIVATE Gears-engine)



cmake_minimum_required(VERSION 3.28.0)
project(Gears-engine-editor VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC_IDE)
    # setting the build type to Release as a fallback when no type is provided during generation
    if(CMAKE_BUILD_TYPE STREQUAL "" OR NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BUILD_TYPE} ${CMAKE_SYSTEM_NAME}")
endif()


# debug
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zi")
    endif()
endif()

file(GLOB SOURCES "gears/editor/*.cpp")

if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    add_executable(Gears ${SOURCES} script.rc)
else()
    add_executable(Gears ${SOURCES})
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    add_definitions(-DGRS_DEBUG)
endif()

#adding modules
file(GLOB MODULES "*.cppm")
target_sources(Gears PUBLIC FILE_SET gears_modules TYPE CXX_MODULES FILES
   ${MODULES}
)

# adding extern libs
add_subdirectory(gears/engine  EXCLUDE_FROM_ALL)
add_subdirectory(gears/editor/vendor EXCLUDE_FROM_ALL) # imgui

target_link_directories(Gears PRIVATE Gears-engine imgui)
target_link_libraries(Gears PRIVATE Gears-engine imgui)




# cpack
install(DIRECTORY "build/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/" DESTINATION .)
if(${CMAKE_SYSTEM} MATCHES Windows)
    set(CPACK_GENERATOR "NSIS;ZIP;7Z;WIX")
endif()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_NAME "Gears game engine")
include(CPack)


