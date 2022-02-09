#!/bin/bash
# c++ server on boost (ssl)
# chmod +x ./compile_redirect.bash
echo "compilation..."
g++ *.cpp -o redirect -pthread -lboost_system -std=c++17
echo "done"