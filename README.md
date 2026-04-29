# SystemC Parametrized Subtractor Project (sampleSub)

## Overview

This project demonstrates a **SystemC High-Level Synthesis (HLS)** implementation of a parametrized (template-based) subtractor module with configurable bit widths and overflow/underflow detection. It showcases advanced C++ template metaprogramming in SystemC context with comprehensive testbenches.

## Project Structure

```
sampleSub/
├── include/
│   ├── subtractor.h                  # Original non-parametrized module (legacy)
│   └── parametrized_subtractor.h    # Template-based parametrized module
├── src/
│   └── subtractor.cpp               # Original implementation (legacy)
├── testbench/
│   ├── tb_subtractor.cpp            # Legacy subtractor testbench
│   └── tb_parametrized.cpp          # Comprehensive parametrized testbench
├── scripts/
│   └── run.sh                       # Build and execution script
├── build/                           # Build output directory
├── CMakeLists.txt                   # CMake configuration
└── README.md                        # This file
```

## Features

### 🔧 Parametrized Subtractor Module
A C++ template-based SystemC module supporting **three configurable bit widths**:

| Bit Width | Data Type | Range |
|-----------|-----------|-------|
| 8-bit | `int8_t` | -128 to 127 |
| 16-bit | `int16_t` | -32,768 to 32,767 |
| 32-bit | `int32_t` | -2,147,483,648 to 2,147,483,647 |

### ✨ Key Features
- **Template-based design**: Single code base for multiple bit widths
- **Overflow Detection**: Identifies when result exceeds maximum value
- **Underflow Detection**: Identifies when result falls below minimum value  
- **Saturation Behavior**: Results are clamped to min/max bounds when overflow/underflow occurs
- **Combinational Logic**: Zero-delay computation using `SC_METHOD`
- **Flag Outputs**: Separate flags for overflow and underflow conditions

## Module Architecture

### Parametrized Subtractor (`include/parametrized_subtractor.h`)

A C++ template `SC_MODULE` that performs signed subtraction with overflow/underflow detection.

**Template Parameters:**
```cpp
template<int BitWidth>  // Valid values: 8, 16, 32
SC_MODULE(parametrized_subtractor)
```

**Ports:**

| Port | Type | Direction | Description |
|------|------|-----------|-------------|
| `a` | `sc_in<DataType>` | Input | First operand (minuend) |
| `b` | `sc_in<DataType>` | Input | Second operand (subtrahend) |
| `difference` | `sc_out<DataType>` | Output | Result of a - b (saturated) |
| `overflow_flag` | `sc_out<bool>` | Output | High when result > MAX |
| `underflow_flag` | `sc_out<bool>` | Output | High when result < MIN |

**Process:**
- **Type**: `SC_METHOD` (combinational logic)
- **Sensitivity**: Triggers on changes to ports `a` or `b`
- **Computation**: 
  - Computes `result = a - b` using 64-bit intermediate representation
  - Detects overflow if `result > MAX_VALUE`
  - Detects underflow if `result < MIN_VALUE`
  - Saturates result to min/max bounds when overflow/underflow occurs

### Example Usage

```cpp
#include "parametrized_subtractor.h"

// Create signals for 8-bit subtractor
sc_signal<int8_t> signal_a, signal_b, signal_diff;
sc_signal<bool>   signal_overflow, signal_underflow;

// Instantiate 8-bit subtractor
parametrized_subtractor<8> sub8("sub_8bit");

// Bind signals to ports
sub8.a(signal_a);
sub8.b(signal_b);
sub8.difference(signal_diff);
sub8.overflow_flag(signal_overflow);
sub8.underflow_flag(signal_underflow);

// Test case: 100 - (-50) with 8-bit (should overflow)
signal_a.write(100);
signal_b.write(-50);
wait(1, SC_NS);

// Results:
// - signal_diff = 127 (saturated maximum)
// - signal_overflow = true
// - signal_underflow = false
```

### Multiple Instances

The parametrized design allows instantiating different bit-width versions simultaneously:

```cpp
// Create 8-bit, 16-bit, and 32-bit instances in same testbench
parametrized_subtractor<8>  sub_8bit("sub_8");
parametrized_subtractor<16> sub_16bit("sub_16");
parametrized_subtractor<32> sub_32bit("sub_32");

// All run in parallel with their respective data types
```

## Test Coverage

### Testbench Overview

The project includes comprehensive testbenches that validate correctness:

#### 1. **Original Subtractor** (`testbench/tb_subtractor.cpp`)
- Legacy non-parametrized version for reference
- Basic arithmetic operations
- Boundary conditions
- Edge cases

#### 2. **Parametrized Subtractor** (`testbench/tb_parametrized.cpp`)
A comprehensive testbench with **4 test groups**:

**TEST 1-3: Basic Operations**
- Normal positive and negative arithmetic
- Zero handling
- Operations testing all bit widths (8, 16, 32-bit)

**TEST 4: Critical Boundary Cases & Saturation**
- Identity operations: `a - a = 0` (should never overflow)
- Min/Max edge cases: `MIN - 0`, `MAX - 0`, etc.
- Flag mutual exclusivity verification
- Saturation verification (result exactly equals MIN or MAX)

### Test Cases for 8-bit Subtractor

| Test | A | B | Expected | Result | OV | UF | Status |
|------|---|---|----------|--------|----|----|--------|
| Normal | 10 | 5 | 5 | 5 | N | N | PASS |
| Negative | -10 | -5 | -5 | -5 | N | N | PASS |
| Zero | 0 | 0 | 0 | 0 | N | N | PASS |
| Overflow | 127 | -1 | 128 → 127 | 127 | Y | N | PASS |
| Underflow | -128 | 1 | -129 → -128 | -128 | N | Y | PASS |

### Running Tests

```bash
# Navigate to project directory
cd path/to/sampleSub

# Build and run all tests
./scripts/run.sh
```

**Output:**
```
==========================================
Subtractor Project - Build & Run Script
==========================================

[1/3] Configuring and building project...
...
[2/3] Running Parametrized Subtractor Testbench
...
[TEST 1] 8-bit Subtractor (Range: -128 to 127)
──────────────────────────────────────────────────
A    | B    | Result | Expected | OV | UF | Status
──────────────────────────────────────────────────
10   |  5   |   5    |    5     | N  | N  | PASS
[more tests...]
8-bit Tests: 17/17 passed
...
All tests completed
```

## Building the Project

### Prerequisites
- **SystemC Library**: Compiled and installed (typically at `C:/msys64/usr/local/systemc`)
- **CMake**: Version 3.10 or later
- **C++17 Compiler**: GCC, Clang, or MSVC with C++17 support

### Build Steps

```bash
# 1. Navigate to project root
cd path/to/sampleSub

# 2. Clean previous builds (optional)
rm -rf build

# 3. Configure and build
cmake -S . -B build
cmake --build build

# 4. Run tests
./build/testbench/tb_parametrized
./build/testbench/tb_subtractor
```

### Using the Run Script

```bash
# Automatically builds and runs all testbenches
./scripts/run.sh
```

## Implementation Details

### Template Specialization

The parametrized subtractor uses C++ template metaprogramming to handle different data types:

```cpp
template<int BitWidth>
struct parametrized_subtractor {
    // Determine data type based on BitWidth
    using DataType = typename std::conditional<
        BitWidth == 8, int8_t,
        typename std::conditional<BitWidth == 16, int16_t, int32_t>::type
    >::type;
    
    // Ports use DataType
    sc_in<DataType> a, b;
    sc_out<DataType> difference;
};
```

### Overflow/Underflow Detection

The implementation uses a 64-bit intermediate representation to detect overflow/underflow safely:

```cpp
int64_t result_extended = static_cast<int64_t>(a_val) - static_cast<int64_t>(b_val);

// Check bounds
bool overflow = (result_extended > MAX_VAL);
bool underflow = (result_extended < MIN_VAL);

// Saturate if needed
if (overflow)
    difference.write(MAX_VAL);
else if (underflow)
    difference.write(MIN_VAL);
else
    difference.write(static_cast<DataType>(result_extended));
```

## Design Patterns Used

1. **Template Metaprogramming**: C++ templates for compile-time polymorphism
2. **SystemC SC_METHOD**: Combinational logic implementation
3. **Signal Binding**: Port connections in hierarchical design
4. **Saturation Arithmetic**: Overflow/underflow handling strategy
5. **Type Traits**: `std::conditional` for conditional type selection

## Performance Characteristics

| Metric | Value |
|--------|-------|
| Propagation Delay | Combinational (0 delay) |
| Throughput | One result per cycle |
| Power | Minimal (combinational only) |
| Area | Proportional to bit width |
| Latency | 1 delta cycle |

## Future Enhancements

- [ ] Add rounding modes for truncation handling
- [ ] Implement pipeline stages for high-frequency designs
- [ ] Add support for unsigned subtraction variants
- [ ] Generate resource estimates for different platforms
- [ ] Add VCD (Value Change Dump) tracing for waveform analysis
- [ ] Implement wrap-around (modulo) behavior option
- [ ] Support for 64-bit integer subtraction

## Files Summary

| File | Purpose |
|------|---------|
| `parametrized_subtractor.h` | Template-based subtractor module (PRIMARY) |
| `subtractor.h` | Legacy non-parametrized version (reference) |
| `tb_parametrized.cpp` | Comprehensive test suite with multiple bit widths |
| `CMakeLists.txt` | Build configuration supporting both versions |
| `run.sh` | Automated build and test execution script |

## References

- [SystemC Language Reference Manual](https://accellera.org/)
- C++ Template Metaprogramming
- IEEE 754 Fixed-Point Arithmetic
- VHDL/Verilog Saturation Arithmetic Standards

## License

This project is provided as an educational example for SystemC HLS learning purposes.

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
