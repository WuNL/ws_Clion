cmake_minimum_required(VERSION 3.10)
project(singleton)

set(CMAKE_CXX_STANDARD 11)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
add_executable(singleton main.cpp Singleton.h)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS
                date_time filesystem iostreams)
target_link_libraries(singleton Threads::Threads)
