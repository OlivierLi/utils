cmake_minimum_required(VERSION 2.8)
project(interceptor)

SET(THREADS_PREFER_PTHREAD_FLAG ON)
SET(CMAKE_CXX_COMPILER clang++)
SET(CLANG_COMPILE_FLAGS "-Wall -Wextra -Wfatal-errors --std=c++14 -g")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CLANG_COMPILE_FLAGS}" )
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FIND_PACKAGE(Threads REQUIRED)

#Find all cpp files recursively
FILE(GLOB_RECURSE SRC "src" "src/*.cpp")

add_executable(utils ${SRC})
#rt for signals and timers.
TARGET_LINK_LIBRARIES(utils rt)

#Threads for pthread_kill/pthread_self
TARGET_LINK_LIBRARIES(utils Threads::Threads)
