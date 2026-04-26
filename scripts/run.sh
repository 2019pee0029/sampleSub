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
echo "[1/3] Configuring and building project..."
rm -rf build
mkdir -p build

# Configure and build using modern CMake syntax
cmake -S . -B build
cmake --build build

# Run parametrized testbench (comprehensive test with boundary/saturation/flag validation)
echo ""
echo "=========================================="
echo "Running Parametrized Subtractor Testbench"
echo "=========================================="
echo ""

if [ -f "build/testbench/tb_parametrized" ] || [ -f "build/testbench/tb_parametrized.exe" ]; then
    if [ -f "build/testbench/tb_parametrized.exe" ]; then
        ./build/testbench/tb_parametrized.exe
    else
        ./build/testbench/tb_parametrized
    fi
    echo ""
else
    echo "ERROR: Parametrized testbench executable not found!"
    exit 1
fi

# Run original testbench (basic tests with parametrized scenarios)
echo ""
echo "=========================================="
echo "Running Original Subtractor Testbench"
echo "=========================================="
echo ""

if [ -f "build/testbench/tb_subtractor" ] || [ -f "build/testbench/tb_subtractor.exe" ]; then
    if [ -f "build/testbench/tb_subtractor.exe" ]; then
        ./build/testbench/tb_subtractor.exe
    else
        ./build/testbench/tb_subtractor
    fi
    echo ""
else
    echo "ERROR: Basic testbench executable not found!"
    exit 1
fi

echo "=========================================="
echo "All tests execution completed"
echo "=========================================="
