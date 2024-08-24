#!/bin/bash

# Exit on any error
set -e

# Directory setup
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$SCRIPT_DIR/build"
WEB_DIR="$SCRIPT_DIR/web"
WASM_SRC_DIR="$SCRIPT_DIR/src"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Create web directory if it doesn't exist
mkdir -p "$WEB_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

#emcmake cmake -DCMAKE_BUILD_TYPE=Release .
#emmake make

# Run CMake
emcmake cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-I$WASM_SRC_DIR/wasm_specific -I$WASM_SRC_DIR/mshv_adapted" \
    "$SCRIPT_DIR"

# Build the project
emmake make

# Copy the output to the web directory
cp mshv_ft8.js mshv_ft8.wasm "$WEB_DIR/"

echo "Build complete. Output files are in $WEB_DIR"