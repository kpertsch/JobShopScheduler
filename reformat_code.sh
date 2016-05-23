#!/bin/sh
clang-format -i -style="{BasedOnStyle: Webkit, BreakBeforeBraces: Allman, IndentWidth: 4, KeepEmptyLinesAtTheStartOfBlocks: true}" include/*.h src/*.cpp

