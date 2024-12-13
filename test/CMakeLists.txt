
project(reader_test_0)

add_executable(${PROJECT_NAME} test_0/main.cpp)

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)

project(reader_test_1)

aux_source_directory(../lib SRC_FILES)
aux_source_directory(../src/config SRC_FILES)
aux_source_directory(../src/bookshelf SRC_FILES)
add_executable(${PROJECT_NAME} test_1/main.cpp ${SRC_FILES})
include_directories(../lib)
include_directories(../src/config)
include_directories(../src/bookshelf)

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
