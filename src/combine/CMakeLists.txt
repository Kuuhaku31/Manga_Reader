# 项目名称
project(combine)

# 添加源文件
add_library(${PROJECT_NAME} SHARED combine.cpp setgroup.cpp)

# 添加EasyX头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC E:/software/lib)

# 添加链接库
target_link_libraries(${PROJECT_NAME} E:/software/lib/libeasyx.a)
