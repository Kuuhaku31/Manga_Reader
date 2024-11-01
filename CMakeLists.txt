# 项目信息
cmake_minimum_required(VERSION 3.20.0)
project(Manga_Reader)

# cpp版本
set(CMAKE_CXX_STANDARD 20)

# 变量设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(Combine_Dir ${PROJECT_SOURCE_DIR}/src/combine)
set(DecodeAvif_Dir ${PROJECT_SOURCE_DIR}/src/decodeavif)

# 子目录
add_subdirectory(src)
