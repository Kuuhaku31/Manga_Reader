# src/test/test_bookshelf/CMakeLists.txt

# 设置二进制文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)

project(test_bookshelf)

set(SRC main.cpp)
aux_source_directory(${MOD_PATH_BOOKSHELF} SRC)
include_directories(
    ${MOD_PATH_BOOKSHELF}
    E:/software/msys64/mingw64/include/cjson
)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE cJSON)
