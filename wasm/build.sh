#!/bin/bash

# Exit on any error
set -e

# Directory setup
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$SCRIPT_DIR/build"
WEB_DIR="$SCRIPT_DIR/web"
SRC_DIR="$SCRIPT_DIR/../src"
WASM_SRC_DIR="$SCRIPT_DIR/src"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Create web directory if it doesn't exist
mkdir -p "$WEB_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake with verbose output
emcmake cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-I$SRC_DIR/HvMsPlayer/libsound/HvPackUnpackMsg" \
    "$SCRIPT_DIR" \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON


emmake make

# Copy the output to the web directory
cp ft8_wasm.js ft8_wasm.wasm "$WEB_DIR/"

echo "Build complete. Output files are in $WEB_DIR"