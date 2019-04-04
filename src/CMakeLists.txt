cmake_minimum_required(VERSION 3.0)
project(html-parser CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC "*.cc")
add_executable(html-parser ${SRC})
target_link_libraries(html-parser stdc++fs)
