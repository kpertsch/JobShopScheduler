cmake_minimum_required(VERSION 2.8)
project(JobShopSchedule)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

file(GLOB SOURCE_FILES src/*.cpp)
add_executable(JobShopSchedule ${SOURCE_FILES})
