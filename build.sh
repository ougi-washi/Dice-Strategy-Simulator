#!/bin/bash

# Create build directory
mkdir build
cd build

# Generate build files using CMake
if command -v ninja &> /dev/null
then
    cmake -G "Ninja" ..
else
    cmake ..
fi

# Build the project
if command -v ninja &> /dev/null
then
    ninja
else
    make
fi