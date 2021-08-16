# hello world example
add_executable("cell" 

  main.cpp
	

)

target_link_libraries("cell" graphics user_input)
target_include_directories("cell" PUBLIC src)
