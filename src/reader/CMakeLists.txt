
cmake_minimum_required(VERSION 3.5)
project(Manga_Reader
    VERSION 0.0.1
    DESCRIPTION "Can support manga reading"
    HOMEPAGE_URL "https://github.com/Kuuhaku31/Manga_Reader"
    LANGUAGES CXX)

# 项目信息↑

# 启用详细的编译和链接输出
set(CMAKE_VERBOSE_MAKEFILE ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置生成的可执行文件的输出路径为bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 添加源文件
# 查找并包含src目录下的全部源文件
file(GLOB_RECURSE SRC_FILES src/*.cpp main.cpp)

# 打印找到的源文件列表
message(STATUS "Source files: ${SRC_FILES}")

# 添加头文件
include_directories(./include)

# 添加EasyX头文件目录
include_directories("E:\\software\\lib")

# 添加EasyX库目录
link_directories("E:\\software\\lib")

# 生成可执行文件
add_executable(a ${SRC_FILES})

# 添加链接库
target_link_libraries(a easyx)
