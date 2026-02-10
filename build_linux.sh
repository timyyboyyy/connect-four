#!/bin/bash
set -euo pipefail

# Linux-only build script (uses g++)
mkdir -p build

CXX="${CXX:-g++}"
CXXFLAGS=(-std=c++17 -O2 -Wall -Wextra -Isrc)

SOURCES=(
  src/main.cpp
)

# Collect all .cpp files from subfolders
for dir in board gameengine gamelogger testsuite util stats mainmenu; do
  while IFS= read -r -d '' f; do
    SOURCES+=("$f")
  done < <(find "src/$dir" -type f -name "*.cpp" -print0 2>/dev/null || true)
done

"$CXX" "${CXXFLAGS[@]}" "${SOURCES[@]}" -o build/connect_four

echo "Build OK (Linux)."
echo "Start: ./run.sh"
