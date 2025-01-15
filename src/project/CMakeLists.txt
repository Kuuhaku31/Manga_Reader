# src/project/CMakeLists.txt

# project(reader)

# 向上传递
set(MOD_PATH_BOOKSHELF ${CMAKE_CURRENT_SOURCE_DIR}/bookshelf PARENT_SCOPE)
set(MOD_PATH_COMBIN_PAGE ${CMAKE_CURRENT_SOURCE_DIR}/combin_page PARENT_SCOPE)

# # 添加路径
# set(PATHS
# config
# bookshelf
# console
# imgui_windows
# combin_page
# imgui_setup
# )

# foreach(PATH ${PATHS})
# file(GLOB SRC_FILES ${SRC_FILES} ${PATH}/*.cpp)
# set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PATH})
# endforeach()

# #
# # 添加imgui
# set(IMGUI_DIR D:/Project/GitHub/imgui)
# set(IMGUI_BACKENDS_DIR D:/Project/GitHub/imgui/backends)
# set(MINGW64_INCLUDE_DIRS E:/software/msys64/mingw64/include)

# aux_source_directory(${IMGUI_DIR} SRC_FILES)
# set(SRC_FILES
# main.cpp
# ${SRC_FILES}
# ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp
# ${IMGUI_BACKENDS_DIR}/imgui_impl_sdlrenderer2.cpp
# )

# foreach(SRC_FILE ${SRC_FILES})
# message(STATUS "Source file: ${SRC_FILE}")
# endforeach()

# add_executable(${PROJECT_NAME} ${SRC_FILES}) # 生成可执行文件

# # 添加头文件
# include_directories(
# ${INCLUDE_DIRS}

# ${MINGW64_INCLUDE_DIRS}/cJSON
# ${MINGW64_INCLUDE_DIRS}/stb
# ${MINGW64_INCLUDE_DIRS}/SDL2

# ${IMGUI_DIR}
# ${IMGUI_BACKENDS_DIR}
# )

# # 添加链接库
# target_link_libraries(${PROJECT_NAME} PRIVATE
# SDL2
# SDL2_image
# cJSON
# )

# # 复制 DLL 文件到输出目录
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_if_different

# E:/software/msys64/mingw64/bin/SDL2.dll
# E:/software/msys64/mingw64/bin/SDL2_image.dll

# $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )
