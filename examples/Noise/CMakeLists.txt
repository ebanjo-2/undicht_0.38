# an example to test out perlin noise and its uses
add_executable("noise"

	main.cpp

)

target_link_libraries("noise" graphics user_input)
target_include_directories("noise" PUBLIC src)
