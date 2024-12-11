
cmake_minimum_required(VERSION 3.10)
project(Manga_Reader
    VERSION 0.0.1
    DESCRIPTION "Can support manga reading"
    HOMEPAGE_URL "https://github.com/Kuuhaku31/Manga_Reader"
    LANGUAGES CXX)

# 项目信息↑

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 启用测试
enable_testing()

# 设置生成的可执行文件的输出路径为bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(lib)

add_subdirectory(src)
add_subdirectory(test)
