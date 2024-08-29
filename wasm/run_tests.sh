#!/bin/bash

# Exit on any error
set -e

# Directory setup
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$SCRIPT_DIR/build_tests"
SRC_DIR="$SCRIPT_DIR/src"

echo "Script dir: $SCRIPT_DIR"
echo "Build dir: $BUILD_DIR"
echo "Source dir: $SRC_DIR"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Compile the test file
echo "Compiling tests..."
g++ -std=c++14 -I"$SRC_DIR" \
    "$SRC_DIR/tests/qstring_tests.cpp" \
    "$SRC_DIR/wasm_specific/QString.cpp" \
    "$SRC_DIR/wasm_specific/QChar.cpp" \
    "$SRC_DIR/wasm_specific/QStringList.cpp" \
    -o qstring_tests

# Run the tests
echo "Running tests..."
./qstring_tests

echo "Tests completed."
