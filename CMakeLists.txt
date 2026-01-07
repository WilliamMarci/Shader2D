cmake_minimum_required(VERSION 3.16) # PCH 需要 CMake 3.16+
project(Shader2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(glfw3 3.3 REQUIRED)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

add_library(stb_image vendor/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC vendor/stb_image)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE vendor/glm)

add_library(spdlog INTERFACE)
target_compile_definitions(spdlog INTERFACE SPDLOG_HEADER_ONLY)
target_include_directories(spdlog INTERFACE vendor/spdlog)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(MyGameClient ${SOURCES})

target_precompile_headers(MyGameClient PRIVATE src/pch.h)

target_link_libraries(MyGameClient
    PRIVATE
    glad        # the glad library we defined earlier
    glfw        # glfw found by the system
    stb_image   # image loading library
    glm         # glm math library
    GL          # Linux graphics driver (libGL)
    dl          # dynamic loader (required by glad)
    spdlog      # logging library
    pthread     # threading library (needed for client/server architecture in the future)
)

message(STATUS "Build setup successful for build type: ${CMAKE_BUILD_TYPE}")