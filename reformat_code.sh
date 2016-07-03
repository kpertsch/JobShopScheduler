#!/bin/sh
clang-format -i -style="{BasedOnStyle: Webkit, BreakBeforeBraces: Allman, IndentWidth: 4, ColumnLimit: 150, KeepEmptyLinesAtTheStartOfBlocks: true}" include/*.h src/*.cpp
