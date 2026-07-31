#!/bin/sh
# build.sh — builds Exercise 2 (run from the repo root)
# usage: ./build.sh          build with -O2
#        ./build.sh clean    remove build artifacts and the module cache
set -e

FLAGS="-std=c++23 -fmodules -O2 -Wall"

if [ "$1" = "clean" ]; then
    rm -rf gcm.cache tools.o measurement
    echo "cleaned"
    exit 0
fi

# the std module only needs to be built once per compiler/flags
if [ ! -f gcm.cache/std.gcm ]; then
    echo "building std module (first time only)..."
    g++ $FLAGS -fsearch-include-path bits/std.cc -c -o /dev/null
fi

g++ $FLAGS -c Tools/Tools.cpp -o tools.o
# g++ $FLAGS Exercise_2/Measurement.cpp tools.o -o measurement
g++ $FLAGS Exercise_3/ListsVsVectors.cpp tools.o -o list_vs_vector

echo "done: ./list_vs_vector"