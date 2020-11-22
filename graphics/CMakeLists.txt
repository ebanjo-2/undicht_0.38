# graphics + window library

add_library(graphics STATIC

	include/core/graphics_core.h
	src/core/glfw_gl33/glfw_gl33_graphics_core.h
	src/core/glfw_gl33/glfw_gl33_graphics_core.cpp
	
	include/window/window.h
	src/window/glfw/glfw_window.h
	src/window/glfw/glfw_window.cpp
)

target_include_directories(graphics PUBLIC include src)

add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/EGL-Registry)

target_link_libraries("graphics" khr_platform)
target_link_libraries("graphics" glfw)
target_link_libraries("graphics" glad)