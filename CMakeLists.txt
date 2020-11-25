# the version i am currently using (required version may be much lower)
cmake_minimum_required(VERSION "3.1.0")

# the name of the project described in this file
project("undicht")

# adding undicht libraries
add_subdirectory("graphics")

# adding example applications
add_subdirectory("examples/HelloWorld")
