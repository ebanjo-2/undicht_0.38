# an example to test out perlin noise and its uses
add_executable("et1" 

	main.cpp 
	
	src/user_interface/button.h 
	src/user_interface/button.cpp 
	
	src/user_interface/ui_element.h 
	src/user_interface/ui_element.cpp 
	
	src/user_interface/ui_base.h 
	src/user_interface/ui_base.cpp 
)

target_link_libraries("et1" graphics user_input)
target_include_directories("et1" PUBLIC src)
