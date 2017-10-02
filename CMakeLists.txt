cmake_minimum_required(VERSION 2.8)

project(ChatServer C CXX)
#开启编译命令行调试
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_COMPILER "g++")
#设置全局编译参数
SET(CMAKE_CXX_FLAGS "-Wall -lpthread -fpermissive -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

MESSAGE(STATUS "CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS} )

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src/database)
add_subdirectory(src/inspect)
add_subdirectory(src/microhttp)
add_subdirectory(src/server)
add_subdirectory(src/drawsth)
