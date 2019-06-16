cmake_minimum_required(VERSION 3.14)
project(divecpp LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.70 REQUIRED COMPONENTS context fiber coroutine thread)
find_package(Catch2 2.8.0 REQUIRED)

add_subdirectory(context)
