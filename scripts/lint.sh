#!/bin/sh
set -eu
cmake --preset host-debug
cmake --build --preset host-debug --verbose
if command -v clang-tidy >/dev/null 2>&1; then
    clang-tidy -p build/host-debug src/core/status.c src/ipc/freshness.c
else
    echo "clang-tidy not installed; compiler warnings remain enforced by CMake"
fi
