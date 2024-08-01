
cmake_minimum_required(VERSION 3.5)
project(Manga_Reader
    VERSION 0.0.1
    DESCRIPTION "can read manga"
    HOMEPAGE_URL "https://github.com/Kuuhaku31/Manga_Reader"
    LANGUAGES CXX)

# 项目信息↑

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 设置生成的可执行文件的输出路径为bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 添加源文件
# 查找并包含src目录下的全部源文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# 打印找到的源文件列表
message(STATUS "Source files: ${SRC_FILES}")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 添加头文件
include_directories(./include)

# 添加链接库
target_link_libraries(${PROJECT_NAME} easyx)

# # 链接到 EasyX 图形库
# include_directories(./easyx)
# link_directories(./easyx)
# link_libraries(./EasyX/libeasyx.a)

# # 如果库文件名和路径正确，尝试直接指定库文件的完整路径
# target_link_libraries(run "${CMAKE_SOURCE_DIR}/easyx/libeasyx.a")
