cmake_minimum_required(VERSION 3.16)
project(Shader2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Dependencies ---
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Threads REQUIRED) # 跨平台线程库

# Vendor: GLAD
add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Vendor: stb_image
add_library(stb_image vendor/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC vendor/stb_image)

# Vendor: GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE vendor/glm)

# Vendor: spdlog
add_library(spdlog INTERFACE)
target_compile_definitions(spdlog INTERFACE SPDLOG_HEADER_ONLY)
target_include_directories(spdlog INTERFACE vendor/spdlog)

# --- Source ---
include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(MyGameClient ${SOURCES})

# --- Precompiled Headers ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
    target_precompile_headers(MyGameClient PRIVATE src/pch.h)
endif()

# --- Linking ---
target_link_libraries(MyGameClient
    PRIVATE
    glad
    glfw
    stb_image
    glm
    OpenGL::GL      # 跨平台 OpenGL
    spdlog
    Threads::Threads # 跨平台线程
)

# Linux 特定链接
if(UNIX AND NOT APPLE)
    target_link_libraries(MyGameClient PRIVATE dl)
endif()

message(STATUS "Build setup successful for: ${CMAKE_SYSTEM_NAME}")
