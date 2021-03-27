# hello world example
add_executable("cell" 

    main.cpp
    

  src/material.h
  src/material.cpp
	
	src/rendering/cell_renderer.h
	src/rendering/cell_renderer.cpp
	src/rendering/cell_shader.glsl


	src/camera/camera_control.h
	src/camera/camera_control.cpp
	
	src/chunks/draw_chunk.h
	src/chunks/draw_chunk.cpp
	
	src/chunks/fast_access_chunk.h
	src/chunks/fast_access_chunk.cpp

	src/chunks/edit_chunk.h
	src/chunks/edit_chunk.cpp

	src/chunks/cell.h
	src/chunks/cell.cpp
	
	src/chunks/acell.h
	src/chunks/acell.cpp
)

target_link_libraries("cell" graphics user_input)
target_include_directories("cell" PUBLIC src)
