cmake_minimum_required(VERSION 3.16)
project(yinix VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 递归收集所有 .cpp 源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(yinix ${SOURCES})

target_include_directories(yinix PRIVATE src)
