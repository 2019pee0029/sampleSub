# SystemC Subtractor Project (sampleSub)

## Overview

This is a **SystemC High-Level Synthesis (HLS) example** demonstrating a combinational logic subtractor module. It mirrors the structure of the original `sampleAdder` project but implements subtraction instead of addition.

## Project Structure

```
sampleSub/
├── include/
│   └── subtractor.h           # Module interface definition
├── src/
│   └── subtractor.cpp         # Module implementation
├── testbench/
│   └── tb_subtractor.cpp      # Simulation testbench
├── scripts/
│   └── run.sh                 # Build and execution script
├── build/                     # Build output directory
├── CMakeLists.txt            # CMake configuration
└── README.md                 # This file
```

## Module Architecture

### Subtractor Module (`include/subtractor.h`)

A SystemC `SC_MODULE` that performs combinational subtraction:

**Ports:**
- `sc_in<int> a` - First operand (minuend)
- `sc_in<int> b` - Second operand (subtrahend)
- `sc_out<int> difference` - Output: a - b

**Process:**
- Type: `SC_METHOD` (combinational logic)
- Sensitivity: Triggers on changes to `a` or `b`
- Computation: `difference = a - b`

### Example Usage

```cpp
subtractor sub("subtractor");
sub.a(signal_a);
sub.b(signal_b);
sub.difference(signal_diff);

signal_a.write(10);
signal_b.write(5);
wait(1, SC_NS);
cout << signal_diff.read();  // Output: 5
```

## Test Bench

### Test Coverage (`testbench/tb_subtractor.cpp`)

The testbench includes **10 comprehensive test cases** covering:

✅ **Positive - Positive**: 10 - 5 = 5  
✅ **Negative - Negative**: -10 - (-5) = -5  
✅ **Positive - Negative**: 10 - (-5) = 15  
✅ **Negative - Positive**: -10 - 5 = -15  
✅ **Zero Cases**: 0 - 0 = 0  
✅ **Large Values**: 100 - 50 = 50  
✅ **Equal Values**: 42 - 42 = 0  
✅ **Various Sign Combinations**

### Expected Output Format

```
A - B = Result [Expected] [PASS/FAIL]
Example: 10 - 5 = 5 [5] [PASS]
```

## Build & Execution

### Prerequisites
- **SystemC Library**: Must be installed and configured
- **CMake**: Version 3.10 or higher
- **C++ Compiler**: Supporting C++11 or later

### Build Commands

#### Using the provided script (recommended):
```bash
cd sampleSub
./scripts/run.sh
```

#### Manual build:
```bash
cd sampleSub
mkdir -p build
cd build
cmake ..
make
./testbench/tb_subtractor
```

## Expected Test Output

```
=== Subtractor Test Bench ===
Format: A - B = Result [Expected] [PASS/FAIL]

10 - 5 = 5 [5] [PASS]
-10 - -5 = -5 [-5] [PASS]
10 - -5 = 15 [15] [PASS]
-10 - 5 = -15 [-15] [PASS]
0 - 0 = 0 [0] [PASS]
100 - 50 = 50 [50] [PASS]
-50 - -100 = 50 [50] [PASS]
42 - 42 = 0 [0] [PASS]
1 - -1 = 2 [2] [PASS]
-100 - 100 = -200 [-200] [PASS]

=== Test Summary ===
Total Tests: 10
Passed: 10
Failed: 0
Success Rate: 100%
```

## Key Design Decisions

1. **Simple Integer Arithmetic**: Uses `int` ports for basic subtraction (no bit-width constraints)
2. **Combinational Logic**: `SC_METHOD` for immediate computation (no clock dependency)
3. **Standard Port Names**: Clear naming (`difference` for output, `a`/`b` for inputs)
4. **Comprehensive Testing**: Covers edge cases including sign changes and zero

## Future Enhancements

Potential next stages for this project:

1. **Parametrized Subtractor** - Template-based with configurable bit widths (8/16/32-bit)
2. **Overflow/Underflow Detection** - Add flags for result saturation
3. **ALU Integration** - Combine with parametrized adder for arithmetic logic unit
4. **Sequential Logic** - Add clock-based subtractor with feedback paths

## File Sizes

- `subtractor.h` - ~400 bytes (module definition)
- `tb_subtractor.cpp` - ~1.5 KB (testbench with vectors)
- Compiled executable - ~2.3 MB (with SystemC library)

## Notes

- This project demonstrates the equivalence of subtraction architecture to addition
- The testbench structure mirrors the `sampleAdder` project for consistency
- All logic is combinational; results are available immediately after input changes
- No overflow/underflow handling in this basic version (planned for parametrized version)

## Related Projects

- **sampleAdder** - Basic and parametrized adder modules
- **sampleALU** - (future) Combined arithmetic logic unit using both adder and subtractor
