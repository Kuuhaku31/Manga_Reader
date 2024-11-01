
project(decodeavif)

add_library(${PROJECT_NAME} SHARED combineavif.cpp aviftopng.cpp)

# 添加依赖
# 查找并链接 AVIF 和 PNG 库
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVIF REQUIRED libavif)
find_package(PNG REQUIRED)

# 添加链接库
target_include_directories(${PROJECT_NAME} PUBLIC ${AVIF_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})

# 添加链接库
target_link_libraries(${PROJECT_NAME} ${AVIF_LIBRARIES} ${PNG_LIBRARIES})
