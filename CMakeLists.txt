cmake_minimum_required(VERSION 3.28)

project(gllibs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 查找glfw，glew，opengl库
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include)

set(TEST_SOURCES src/test.cpp)
set(LIB_SOURCES src/gls/shader.cpp src/gls/shader.cpp src/gls/mesh.cpp src/gls/vertex.cpp)

add_executable(gllib_test ${TEST_SOURCES} ${LIB_SOURCES})
add_library(gllibs STATIC ${LIB_SOURCES})

target_link_libraries(gllib_test glfw GLEW::GLEW OpenGL::GL)
target_link_libraries(gllibs glfw GLEW::GLEW OpenGL::GL)
