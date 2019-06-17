project(datetime)

add_executable(datetime_client client.cpp)
target_include_directories(datetime_client PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(datetime_client ${Boost_LIBRARIES})

add_executable(datetime_server server.cpp)
target_include_directories(datetime_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(datetime_server ${Boost_LIBRARIES})

add_executable(datetime_server_async server_async.cpp)
target_include_directories(datetime_server_async PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(datetime_server_async ${Boost_LIBRARIES})
