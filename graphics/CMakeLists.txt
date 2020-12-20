# graphics + window library

add_library(graphics STATIC

	include/core/graphics_core.h
	src/core/glfw_gl33/glfw_gl33_graphics_core.h
	src/core/glfw_gl33/glfw_gl33_graphics_core.cpp
	
	include/window/window.h
	src/window/glfw/glfw_window.h
	src/window/glfw/glfw_window.cpp
	
	include/core/frame_buffer.h
	src/core/gl33/gl33_frame_buffer.h
	src/core/gl33/gl33_frame_buffer.cpp
	
	include/core/renderer.h
	src/core/gl33/gl33_renderer.h
	src/core/gl33/gl33_renderer.cpp
	
	include/core/shader.h
	src/core/gl33/gl33_shader.h
	src/core/gl33/gl33_shader.cpp
	
	include/core/texture.h
	src/core/gl33/gl33_texture.h
	src/core/gl33/gl33_texture.cpp
	
	include/core/uniform.h
	src/core/gl33/gl33_uniform.h
	src/core/gl33/gl33_uniform.cpp
	
	include/core/vertex_buffer.h
	src/core/gl33/gl33_vertex_buffer.h
	src/core/gl33/gl33_vertex_buffer.cpp

    src/core/gl33/gl33_tools.cpp
    src/core/gl33/gl33_tools.h
	
	src/camera/camera_3d.h
	src/camera/camera_3d.cpp
		
	src/camera/perspective_camera_3d.h
	src/camera/perspective_camera_3d.cpp
)

target_include_directories(graphics PUBLIC include src)

add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/EGL-Registry)

target_link_libraries("graphics" EGL-Registry) # khrplatform.h
target_link_libraries("graphics" glfw)
target_link_libraries("graphics" glad)
target_link_libraries("graphics" tools)
