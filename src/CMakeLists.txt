
project(reader)

# 项目信息↑

# 添加源文件
aux_source_directory(./ SRC_FILES)

# 打印找到的源文件列表
message(STATUS "Source files: ${SRC_FILES}")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 添加头文件
include_directories(./)

# 添加链接库
target_link_libraries(${PROJECT_NAME} easyx)
