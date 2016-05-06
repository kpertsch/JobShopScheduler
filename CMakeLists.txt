cmake_minimum_required(VERSION 3.3)
project(JobShopSchedule)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES src/scheduleSolver.cpp src/SolutionGenerator.cpp src/Evaluator.cpp src/RandomSearch.cpp)
add_executable(JobShopSchedule ${SOURCE_FILES})
