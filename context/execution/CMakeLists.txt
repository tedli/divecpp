add_executable(execution main.cpp)
target_include_directories(execution PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(execution ${Boost_LIBRARIES} Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(execution)
