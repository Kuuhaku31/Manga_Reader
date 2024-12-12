
project(reader_test_0)

add_executable(${PROJECT_NAME} test_0/main.cpp)

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)

project(reader_test_1)

add_executable(${PROJECT_NAME} test_1/main.cpp)

# 添加SDL2库
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
