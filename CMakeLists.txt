cmake_minimum_required(VERSION 3.25)
project(threadPool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp src/ThreadPool.cpp)