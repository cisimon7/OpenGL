cmake_minimum_required(VERSION 3.21)
project(OpenGL)

set(CMAKE_CXX_STANDARD 23)
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/dependencies/GLFW)

option(GLFW_BUILD_EXAMPLES "Build the GLFW examples programs" ON)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" ON)
set(GLFW_BUILD_EXAMPLES ON)
set(GLFW_BUILD_TESTS OFF)

add_subdirectory(${GLFW_DIR})

add_executable(OpenGL main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/src)
target_link_libraries(${PROJECT_NAME} glfw)

#cmake -D CMAKE_BUILD_TYPE=Release -D GLFW_NATIVE_API=1 -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64" -D BUILD_SHARED_LIBS=ON -D CMAKE_C_COMPILER=clang ../
