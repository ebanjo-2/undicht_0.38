# hello world example
add_executable("cell" 

  main.cpp
  
  src/player/player.h
  src/player/player.cpp
  
  src/rendering/cell_renderer.h
  src/rendering/cell_renderer.cpp

  src/rendering/texture_atlas.h
  src/rendering/texture_atlas.cpp
  
  src/world/cell.h
  src/world/cell.cpp

  src/world/chunk.h
  src/world/chunk.cpp

  src/math/cell_math.h
  src/math/cell_math.cpp


)

target_link_libraries("cell" graphics user_input)
target_include_directories("cell" PUBLIC src)
