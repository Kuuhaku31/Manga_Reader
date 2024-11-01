# 设置项目名称
project(plannging_main)

# 子目录
add_subdirectory(combine)
add_subdirectory(decodeavif)

# 生成可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 添加依赖
target_include_directories(${PROJECT_NAME} PUBLIC ${Combine_Dir})
target_include_directories(${PROJECT_NAME} PUBLIC ${DecodeAvif_Dir})

target_link_libraries(${PROJECT_NAME} PUBLIC combine decodeavif)
