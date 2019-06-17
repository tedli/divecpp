project(timer)

add_executable(timer main.cpp)
target_include_directories(timer PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(timer ${Boost_LIBRARIES} Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(timer)
