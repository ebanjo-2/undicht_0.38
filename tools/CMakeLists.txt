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
	
	src/files/file.h
	src/files/file.cpp
	
	src/files/xml/xml_element.h
	src/files/xml/xml_element.cpp
	
	src/files/xml/xml_file.h
	src/files/xml/xml_file.cpp
	
	src/files/xml/xml_tag_attribute.h
	src/files/xml/xml_tag_attribute.cpp
	
	src/files/model_loading/model_loader.h
	src/files/model_loading/model_loader.cpp
	
	src/files/model_loading/collada/collada_file.h
	src/files/model_loading/collada/collada_file.cpp
	
	src/math/orientation_3d.h
	src/math/orientation_3d.cpp

	src/math/orthographic_projection.h
	src/math/orthographic_projection.cpp
	
	src/math/perspective_projection.h
	src/math/perspective_projection.cpp

)

target_include_directories("tools" PUBLIC src extern/glm)

add_subdirectory(extern/stb)
target_link_libraries("tools" PUBLIC stb_image)
