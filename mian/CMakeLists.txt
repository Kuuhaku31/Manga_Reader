
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(main)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置构建类型为 Debug
set(CMAKE_BUILD_TYPE Debug)

# 添加源文件
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp main.cpp)

# 打印源文件
message(STATUS "SOURCES: ${SOURCES}")

# 设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 生成可执行文件
add_executable(a ${SOURCES})
