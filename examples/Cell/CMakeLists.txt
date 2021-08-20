# hello world example
add_executable("cell" 

  main.cpp
  
  src/player/player.h
  src/player/player.cpp
  
  src/rendering/cell_renderer.h
  src/rendering/cell_renderer.cpp
  
  src/world/cell.h
  src/world/cell.cpp
	

)

target_link_libraries("cell" graphics user_input)
target_include_directories("cell" PUBLIC src)
