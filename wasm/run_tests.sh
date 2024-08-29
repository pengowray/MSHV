#!/bin/bash

# to run with qt:
# $ run_tests.sh qt

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

# Check if we're using Qt or custom implementation
if [ "$1" = "qt" ]; then
    echo "Compiling tests with Qt..."
    g++ -std=c++14 -fPIC \
        $(pkg-config --cflags Qt5Core) \
        -I"$SRC_DIR" \
        "$SRC_DIR/tests/qstring_tests.cpp" \
        $(pkg-config --libs Qt5Core) \
        -DQT_IMPL \
        -o qstring_tests
else
    echo "Compiling tests with custom implementation..."
    g++ -std=c++14 -I"$SRC_DIR" \
        "$SRC_DIR/tests/qstring_tests.cpp" \
        "$SRC_DIR/qtext_simple/QString.cpp" \
        "$SRC_DIR/qtext_simple/QChar.cpp" \
        "$SRC_DIR/qtext_simple/QStringList.cpp" \
        -o qstring_tests
fi

# Run the tests
echo "Running tests..."
./qstring_tests

echo "Tests completed."