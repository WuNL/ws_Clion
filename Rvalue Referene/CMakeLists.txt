cmake_minimum_required(VERSION 3.12)
project(Rvalue_Referene)

set(CMAKE_CXX_FLAGS "-fno-elide-constructors")

set(CMAKE_CXX_STANDARD 11)

add_executable(Rvalue_Referene main.cpp)