#!/bin/sh

cd build && cmake .. && make && cd .. && mkdir -p solutions && echo -e "Starting program:\n" && build/jss
