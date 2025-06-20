#!/bin/bash

cmake -G Ninja -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# cp build/compile_commands.json .
cmake --build build
