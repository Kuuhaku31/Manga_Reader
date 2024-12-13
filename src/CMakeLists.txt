
project(reader)

# 项目信息↑

# 添加源文件
aux_source_directory(./ SRC_FILES)
aux_source_directory(./config SRC_FILES)
aux_source_directory(../lib SRC_FILES)

# 打印找到的源文件列表
message(STATUS "Source files: ${SRC_FILES}")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 添加头文件
include_directories(./)
include_directories(./config)

# 添加链接库
# 查找cJSON
find_package(cJSON REQUIRED)

if(cJSON_FOUND)
    message(STATUS "==cJSON FOUND==")
    message(STATUS "CJSON_INCLUDE_DIRS: ${CJSON_INCLUDE_DIRS}")
    message(STATUS "CJSON_LIBRARIES: ${CJSON_LIBRARIES}")

    include_directories(${CJSON_INCLUDE_DIRS}/cjson)
    target_link_libraries(${PROJECT_NAME} ${CJSON_LIBRARIES})
else()
    message(FATAL_ERROR "cJSON not found!")
endif()

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)
