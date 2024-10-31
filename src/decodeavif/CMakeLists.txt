
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(aviftopng)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置构建类型为 Debug
set(CMAKE_BUILD_TYPE Debug)

# 启用详细的编译和链接输出
# set(CMAKE_VERBOSE_MAKEFILE ON)

# 查找并链接 AVIF 库
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVIF REQUIRED libavif)

# 查找并链接 libpng 库
find_package(PNG REQUIRED)

# 添加EasyX头文件目录
include_directories("E:\\software\\lib")

# 添加EasyX库目录
link_directories("E:\\software\\lib")

# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include ${AVIF_INCLUDE_DIRS})

# 添加源文件
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp main.cpp)

# 打印源文件
message(STATUS "SOURCES: ${SOURCES}")

# 设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接 AVIF 和 EASYX 库
target_link_libraries(${PROJECT_NAME} ${AVIF_LIBRARIES} ${PNG_LIBRARIES} easyx)
