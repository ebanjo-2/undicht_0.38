# graphics + window library

add_library(graphics STATIC
	include/graphics_core.h
	src/graphics_core.cpp
)

target_include_directories(graphics PUBLIC include)