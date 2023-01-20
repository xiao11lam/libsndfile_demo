cmake_minimum_required(VERSION 3.16)
project(test)
#set(INC_DIR /libsndfile/include)

set(CMAKE_CXX_STANDARD 17)

add_executable(test main.cpp)

target_link_libraries(test ${CMAKE_SOURCE_DIR}/lib/libsndfile.so)