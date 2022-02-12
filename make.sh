#!/bin/bash
if ! [ -f "$1.cpp" ]; then
	echo "You need to pass me the name of a file (minus the .cpp)"
	exit 1;
fi
mkdir -p bin obj
g++ -Wall -fexceptions -O3 -std=c++17 -march=native  -c "./$1.cpp" -o "obj/$1.o"
g++ -o "bin/$1" "obj/$1.o" -s
