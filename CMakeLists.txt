cmake_minimum_required(VERSION 2.8)
project(JobShopSchedule)

find_package(OpenMP REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(CAIRO REQUIRED cairomm-1.0)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_executable(j3s ${SOURCE_FILES} ${HEADER_FILES})
add_definitions(-std=c++11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

include_directories(include ${CAIRO_INCLUDE_DIRS})
target_link_libraries(j3s ${CAIRO_LIBRARIES})
