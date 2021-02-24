# hello world example
add_executable("cell" 

    main.cpp
    
    src/cell.h
    
    src/chunk.h

    src/material.h
    src/material.cpp
	
	src/rendering/cell_renderer.h
	src/rendering/cell_renderer.cpp
	src/rendering/cell_shader.glsl

	src/rendering/chunk_buffer.h
	src/rendering/chunk_buffer.cpp

	src/camera/camera_control.h
	src/camera/camera_control.cpp

)

target_link_libraries("cell" graphics user_input)
target_include_directories("cell" PUBLIC src)
