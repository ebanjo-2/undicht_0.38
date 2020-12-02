add_library("tools" 
	src/buffer_layout.h
	src/buffer_layout.cpp
	
	src/event_logger.h
	src/event_logger.cpp	
	
	src/memory_watcher.h
	src/memory_watcher.cpp	
	
	src/note.h
	src/note.cpp	
	
	src/shared.h
	
	src/shared_id.h
	src/shared_id.cpp	
	
	src/string_tools.h
	src/string_tools.cpp	
	
	src/types.h
	src/types.cpp	
)

target_include_directories("tools" PUBLIC src)