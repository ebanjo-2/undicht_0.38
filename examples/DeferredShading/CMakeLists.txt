# deferred_shading example
add_executable("deferred_shading" 

	main.cpp 

	src/camera_control.h
	src/camera_control.cpp
	
	src/scene.h
	src/scene.cpp
)

target_link_libraries("deferred_shading" graphics user_input)
target_include_directories("deferred_shading" PUBLIC src)