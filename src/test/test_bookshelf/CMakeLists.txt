# src/test/test_bookshelf/CMakeLists.txt

# 设置二进制文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)

project(test_bookshelf)

set(SRC main.cpp)
aux_source_directory(${MOD_PATH_BOOKSHELF} SRC)
aux_source_directory(${MOD_PATH_COMBIN_PAGE} SRC)
include_directories(
    ${THIRD_PARTY_INC_PATHS}

    ${MOD_PATH_BOOKSHELF}
    ${MOD_PATH_COMBIN_PAGE}
    E:/software/msys64/mingw64/include/stb
    E:/software/msys64/mingw64/include/SDL2
    E:/software/msys64/mingw64/include/avif
)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE
    third_party

    avif
)
