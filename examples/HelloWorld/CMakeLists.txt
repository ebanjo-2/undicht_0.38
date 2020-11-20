# hello world example
add_executable("hello_world" main.cpp)

target_link_libraries("hello_world" graphics)
# target_include_directories("hello_world" PUBLIC ${PROJECT_SOURCE_DIR}/graphics/include)