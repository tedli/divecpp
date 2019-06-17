project(process_per_connection)

add_executable(process_per_connection main.cpp)
target_include_directories(process_per_connection PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(process_per_connection ${Boost_LIBRARIES})
