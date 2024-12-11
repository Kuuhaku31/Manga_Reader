
project(reader_test_0)

add_executable(${PROJECT_NAME} test_0/main.cpp)

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)
