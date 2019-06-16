set(Catch2_DIR)
find_package(Catch2 REQUIRED 2.8.0)

add_executable(continuation main.cpp continuation.hpp)
target_include_directories(continuation PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(continuation ${Boost_LIBRARIES} Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(continuation)
