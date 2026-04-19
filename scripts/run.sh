#!/bin/bash

# Build and run SystemC Subtractor Testbench

# Navigate to project root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Change to project directory
cd "$PROJECT_ROOT"

echo "=========================================="
echo "Subtractor Project - Build & Run Script"
echo "=========================================="
echo ""

# Clean previous build
echo "[1/2] Configuring and building project..."
rm -rf build
mkdir -p build

# Configure and build using modern CMake syntax
cmake -S . -B build
cmake --build build

# Run testbench
echo ""
echo "=========================================="
echo "Running Subtractor Testbench"
echo "=========================================="
echo ""

if [ -f "build/testbench/tb_subtractor" ] || [ -f "build/testbench/tb_subtractor.exe" ]; then
    if [ -f "build/testbench/tb_subtractor.exe" ]; then
        ./build/testbench/tb_subtractor.exe
    else
        ./build/testbench/tb_subtractor
    fi
    echo ""
    echo "=========================================="
    echo "Test execution completed"
    echo "=========================================="
else
    echo "ERROR: Testbench executable not found!"
    exit 1
fi
