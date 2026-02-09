#!/bin/bash
set -e
mkdir -p build

g++ -std=c++17 -O2 -Wall -Wextra \
  -Isrc \
  src/main.cpp \
  src/board/*.cpp \
  src/gameengine/*.cpp \
  src/gamelogger/*.cpp \
  src/testsuite/*.cpp \
  src/util/*.cpp \
  -o build/vier_gewinnt


echo "Build OK."
echo "Start: ./run.sh"
