
project(third_party)

set(PATH
    ${CMAKE_CURRENT_LIST_DIR}/base
    ${CMAKE_CURRENT_LIST_DIR}/imgui_setup
)

foreach(p ${PATH})
    aux_source_directory(${p} DIR_LIB_SRCS)
endforeach()

include_directories(${PATH})

# 添加依赖
set(IMGUI_DIR D:/Projects/GitHub/imgui)
set(IMGUI_BACKENDS_DIR D:/Projects/GitHub/imgui/backends)
aux_source_directory(${IMGUI_DIR} IMGUI_SOURCEFILES)
set(IMGUI_SOURCEFILES
    ${IMGUI_SOURCEFILES}
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdlrenderer2.cpp
)

add_library(${PROJECT_NAME}
    ${DIR_LIB_SRCS}
    ${IMGUI_SOURCEFILES}
)

include_directories(
    ${PATH}
    ${IMGUI_DIR}
    ${IMGUI_BACKENDS_DIR}
)

# 添加SDL2库、SDL_image库、SDL2_gfx库、SDL2_ttf库、SDL2_mixer库
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL_image REQUIRED SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

if(SDL2_FOUND)
    message(STATUS "==SDL2 FOUND==")
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

if(SDL_image_FOUND)
    message(STATUS "==SDL_image FOUND==")
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_image_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL_image_LIBRARIES})
else()
    message(FATAL_ERROR "SDL_image not found!")
endif()

if(SDL2_GFX_FOUND)
    message(STATUS "==SDL2_GFX FOUND==")
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_GFX_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_GFX_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2_gfx not found!")
endif()

if(SDL2_TTF_FOUND)
    message(STATUS "==SDL2_TTF FOUND==")
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

if(SDL2_MIXER_FOUND)
    message(STATUS "==SDL2_MIXER FOUND==")
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_MIXER_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_MIXER_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2_mixer not found!")
endif()

# 添加cJSON库
set(cJSON_INCLUDE_DIRS "E:/software/msys64/mingw64/include/cjson")
set(cJSON_LIBRARIES "E:/software/msys64/mingw64/lib/libcjson.a")
target_include_directories(${PROJECT_NAME} PUBLIC ${cJSON_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${cJSON_LIBRARIES})

# 设置include目录
set(THIRD_PARTY_INC_PATHS
    ${PATH}
    ${IMGUI_DIR}
    ${IMGUI_BACKENDS_DIR}
    ${cJSON_INCLUDE_DIRS}
    PARENT_SCOPE
)
