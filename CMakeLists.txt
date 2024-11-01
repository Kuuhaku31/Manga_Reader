# 项目信息
cmake_minimum_required(VERSION 3.5)
project(Manga_Reader)

# cpp版本
set(CMAKE_CXX_STANDARD 20)

# 调试
set(CMAKE_BUILD_TYPE Debug)

# 变量设置
# 会传递给子目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(Combine_Dir ${PROJECT_SOURCE_DIR}/src/combine)
set(DecodeAvif_Dir ${PROJECT_SOURCE_DIR}/src/decodeavif)

# 子目录
add_subdirectory(src)
