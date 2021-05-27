#!/bin/bash

code="$PWD"
opts="-g -std=c++11 -Wall"
cd build > /dev/null
echo  -- building ex2 \(g++ $opts\) --
g++ $opts $code/library2.cpp $code/DynamicArray.cpp $code/RankedTree.cpp $code/HashTable.cpp $code/main2.cpp -o ex2
cd $code > /dev/null
