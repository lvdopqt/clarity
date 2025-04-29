#!/bin/bash

# Configuration
PROJECT_NAME="clarity"
ENTRY_POINT="src/main.cpp"
BUILD_DIR="build"
OUTPUT="$BUILD_DIR/$PROJECT_NAME"

mkdir -p "$BUILD_DIR"

clang++ "$ENTRY_POINT" -o "$OUTPUT"

echo "Compilation done. Output: $OUTPUT"