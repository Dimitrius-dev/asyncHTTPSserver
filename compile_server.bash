#!/bin/bash
# c++ server on boost (ssl)
# chmod +x ./compile_server.bash
echo "compilation..."
g++ *.cpp -o server -pthread -lboost_system -lcrypto -lssl -std=c++17
echo "done"