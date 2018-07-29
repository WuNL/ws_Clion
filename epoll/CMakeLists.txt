cmake_minimum_required(VERSION 3.12)
project(epoll)

set(CMAKE_CXX_STANDARD 11)

add_executable(epoll_server main.cpp)
add_executable(epoll_client client.cpp)