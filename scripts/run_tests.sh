#!/bin/sh
set -eu
cmake --preset host-debug
cmake --build --preset host-debug
ctest --preset host-debug --output-on-failure
