cmake_minimum_required(VERSION 3.10)
project(Shader2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# dependency paths
find_package(glfw3 3.3 REQUIRED)
add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)
include_directories(vendor/glm)


include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(MyGameClient ${SOURCES})

target_link_libraries(MyGameClient
    PRIVATE
    glad        # the glad library we defined earlier
    glfw        # glfw found by the system
    GL          # Linux graphics driver (libGL)
    dl          # dynamic loader (required by glad)
    pthread     # threading library (needed for client/server architecture in the future)
)

message(STATUS "Build setup successful for build type: ${CMAKE_BUILD_TYPE}")
