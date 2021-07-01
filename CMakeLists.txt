cmake_minimum_required(VERSION 3.17)
project(http_simple_server)

set(CMAKE_CXX_STANDARD 98)

add_executable(http_simple_server
        Server.hpp
        Server.cpp
        main.cpp)