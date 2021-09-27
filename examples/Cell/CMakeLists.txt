# hello world example
add_executable("cell" 

  main.cpp
  
  src/player/player.h
  src/player/player.cpp
  
  src/rendering/cell_renderer.h
  src/rendering/cell_renderer.cpp

  src/rendering/cell_geometry_renderer.h
  src/rendering/cell_geometry_renderer.cpp

  src/rendering/cell_light_renderer.h
  src/rendering/cell_light_renderer.cpp

  src/rendering/cell_final_renderer.h
  src/rendering/cell_final_renderer.cpp

  src/rendering/texture_atlas.h
  src/rendering/texture_atlas.cpp
  
  src/world/cell.h
  src/world/cell.cpp

  src/world/chunk.h
  src/world/chunk.cpp
  
  src/world/chunk_optimization.h
  src/world/chunk_optimization.cpp
  
  src/world/world.h
  src/world/world.cpp

  src/math/cell_math.h
  src/math/cell_math.cpp
  
  src/worldgen/world_generator.h
  src/worldgen/world_generator.cpp

)

target_link_libraries("cell" graphics user_input)
target_include_directories("cell" PUBLIC src)
