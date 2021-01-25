add_library("user_input" 
	
	include/key_ids.h
	include/key_ids.cpp
	
	src/glfw/glfw_key_ids.h
	src/glfw/glfw_key_ids.cpp
	
	include/key_input.h
	include/key_input.cpp
	
	src/glfw/glfw_key_input.h
	src/glfw/glfw_key_input.cpp
	
	include/mouse_input.h
	include/mouse_input.cpp
	
	src/glfw/glfw_mouse_input.h
	src/glfw/glfw_mouse_input.cpp

)

target_include_directories("user_input" PUBLIC include src)

target_link_libraries("user_input" graphics)
target_link_libraries("user_input" EGL-Registry) # khrplatform.h
target_link_libraries("user_input" glfw)
