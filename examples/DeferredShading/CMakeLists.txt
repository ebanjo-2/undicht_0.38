# deferred_shading example
add_executable("deferred_shading" 

	main.cpp 

	src/camera_control.h
	src/camera_control.cpp
	
	src/scene.h
	src/scene.cpp
	
	src/gbuffer_renderer.h
	src/gbuffer_renderer.cpp
	
	src/light_renderer.h
	src/light_renderer.cpp
	
	src/final_renderer.h
	src/final_renderer.cpp
)

target_link_libraries("deferred_shading" graphics user_input)
target_include_directories("deferred_shading" PUBLIC src)