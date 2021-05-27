#!/bin/bash

code="$PWD"
opts="-g -std=c++11 -Wall"
cd build > /dev/null
echo  -- building ex1 \(g++ $opts\) --
g++ $opts $code/AvlTree.cpp $code/library.cpp $code/LinkedList.cpp $code/main1.cpp $code/node.cpp -o ex1
cd $code > /dev/null
