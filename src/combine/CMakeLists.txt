# 指定CMake的最低版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(MangaReader_test)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 启用调试信息
set(CMAKE_BUILD_TYPE Debug)

# 设置生成的可执行文件的输出路径为bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 添加源文件
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp main.cpp)

# 打印找到的源文件列表
message(STATUS "Source files: ${SRC_FILES}")

# 添加头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加EasyX头文件目录
include_directories("E:\\software\\lib")

# 添加EasyX库目录
link_directories("E:\\software\\lib")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 添加链接库
target_link_libraries(${PROJECT_NAME} easyx)
