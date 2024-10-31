
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MangaReader)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加子模块路径
set(COMBINEPATH ${PROJECT_SOURCE_DIR}/combine)
set(DECODEAVIFPATH ${PROJECT_SOURCE_DIR}/decodeavif)

# 添加子模块
add_subdirectory(${COMBINEPATH})
add_subdirectory(${DECODEAVIFPATH})

# 输出exe路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加源文件
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp)

# 添加头文件
include_directories(${PROJECT_SOURCE_DIR}/combine/include ${PROJECT_SOURCE_DIR}/decodeavif/include)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
