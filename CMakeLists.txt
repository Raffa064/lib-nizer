cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Nizer)

file(GLOB_RECURSE SOURCE "src/*.cpp")
add_library(nizer ${SOURCE})
target_include_directories(nizer PUBLIC "${PROJECT_SOURCE_DIR}/include")
