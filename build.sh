#!/bin/bash

# Functions
if [ "$1" == "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf $BUILD_DIR
    exit 0
fi

# Project directories
SRC_DIR="./src"
BUILD_DIR="./build"
INCLUDE_DIR="./include"
LIB_DIR="./lib"

# Output executable 
OUTPUT="$BUILD_DIR/Game"

# Compiler and Flags
CXX_LINUX="gcc"
CXX_WINDOWS="x86_64-w64-mingw32-gcc"
CXXFLAGS="-I$INCLUDE_DIR -Wall -O2"
LDFLAGS_WINDOWS="-L$LIB_DIR -lSDL2"
LDFLAGS_LINUX="-lSDL2"

# Check for OS
if [ "$1" == "windows" ]; then
    echo "Cross-compiling for Windows"
    CXX=$CXX_WINDOWS
    LDFLAGS=$LDFLAGS_WINDOWS
elif  [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    echo "Linux detected"
    CXX=$CXX_LINUX
    LDFLAGS=$LDFLAGS_LINUX
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    echo "Windows detected"
    CXX=$CXX_WINDOWS
    LDFLAGS=$LDFLAGS_WINDOWS
else
    echo "Unsupported OS"
    exit 1
fi

# Create build directory if it doesn't exist
mkdir -p $BUILD_DIR

# Find all source files in src/ directory
SOURCES=$(find $SRC_DIR -name "*.c")

# Compile all source files
echo "Compiling and linking with $CXX..."
$CXX $CXXFLAGS $SOURCES $LDFLAGS -o $OUTPUT

# Check for success
if [ $? -eq 0 ]; then
    echo "Build successful"
else
    echo "Build failed"
    exit 1
fi
