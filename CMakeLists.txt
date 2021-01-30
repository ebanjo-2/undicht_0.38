# the version i am currently using (required version may be much lower)
cmake_minimum_required(VERSION "3.1.0")

# the name of the project described in this file
project("undicht")


# adding undicht libraries
add_subdirectory("core")
add_subdirectory("tools")
add_subdirectory("graphics")
add_subdirectory("user_input")

# adding example applications
add_subdirectory("examples/HelloWorld")
add_subdirectory("examples/DeferredShading")
add_subdirectory("examples/Cell")
