# reader_test_combine_page
project(reader_test_combine_page)

# 添加路径
set(PATHS
    ../../src/combin_page
    ../../src/bookshelf
    ../../src/imgui_setup
)

foreach(PATH ${PATHS})
    file(GLOB SRC_FILES ${SRC_FILES} ${PATH}/*.cpp)
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PATH})
endforeach()

set(IMGUI_DIR D:/Project/GitHub/imgui)
set(IMGUI_BACKENDS_DIR D:/Project/GitHub/imgui/backends)
set(MINGW64_INCLUDE_DIRS E:/software/msys64/mingw64/include)

aux_source_directory(${IMGUI_DIR} SRC_FILES)
set(SRC_FILES
    main.cpp
    ${SRC_FILES}
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdlrenderer2.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES}) # 生成可执行文件

# 添加头文件
include_directories(
    ${INCLUDE_DIRS}

    ${MINGW64_INCLUDE_DIRS}/cJSON
    ${MINGW64_INCLUDE_DIRS}/stb
    ${MINGW64_INCLUDE_DIRS}/SDL2

    ${IMGUI_DIR}
    ${IMGUI_BACKENDS_DIR}
)

# 添加链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2
    SDL2_image
    cJSON
)
