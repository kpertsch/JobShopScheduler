#!/bin/sh

mkdir -p build
mkdir -p solutions
cd build && cmake .. && make && cp j3s .. && cd ..
rm -rf build
