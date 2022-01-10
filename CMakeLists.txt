cmake_minimum_required(VERSION 3.21)
project(OpenGL)

set(CMAKE_CXX_STANDARD 23)
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/dependencies/GLFW)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_DIR})

add_executable(OpenGL src/main.cpp src/Renderer.cpp src/Renderer.h src/VertexBuffer.cpp src/VertexBuffer.h src/IndexBuffer.cpp src/IndexBuffer.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include ${GLFW_DIR}/deps)
target_link_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/src)
target_link_libraries(${PROJECT_NAME} glfw)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -I/opt/local/include/ -L/opt/local/lib")
endif()

#cmake -D CMAKE_BUILD_TYPE=Release -D GLFW_NATIVE_API=1 -D CMAKE_OSX_ARCHITECTURES="x86_64;arm64" -D BUILD_SHARED_LIBS=ON -D CMAKE_C_COMPILER=clang ../
