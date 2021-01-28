# hello world example
add_executable("cell" 

    main.cpp
    
    src/cell.h
    
    src/chunk.h

    src/material.h
    src/material.cpp

)

target_link_libraries("cell" graphics)
target_include_directories("cell" PUBLIC src)
