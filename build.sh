#!/bin/bash

# Configuration
PROJECT_NAME="clarity"
ENTRY_POINT="src/main.cpp"
BUILD_DIR="build"
OUTPUT="$BUILD_DIR/$PROJECT_NAME"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Compile using clang++ for the native M1 architecture
clang++ "$ENTRY_POINT" -o "$OUTPUT"

echo "Compilation done. Output: $OUTPUT"