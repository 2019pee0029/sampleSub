cmake_minimum_required(VERSION 3.10)
project(sampleSub)

# Set SYSTEMC_HOME path as per local installation
set(SYSTEMC_HOME "C:/msys64/usr/local/systemc")
set(ENV{SYSTEMC_HOME} ${SYSTEMC_HOME})

# Set C++ standard to C++17 (required by SystemC)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${SYSTEMC_HOME}/include)

# Define SystemC libraries
set(SYSTEMC_LIBRARIES ${SYSTEMC_HOME}/lib/libsystemc.a)

# ============================================================================
# ORIGINAL SUBTRACTOR - Legacy non-parametrized version
# ============================================================================
set(TESTBENCH_ORIGINAL
    testbench/tb_subtractor.cpp
    src/subtractor.cpp
)

add_executable(tb_subtractor ${TESTBENCH_ORIGINAL})
target_link_libraries(tb_subtractor ${SYSTEMC_LIBRARIES})

set_target_properties(tb_subtractor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testbench
)

# ============================================================================
# PARAMETRIZED SUBTRACTOR - Template-based with configurable bit widths
# ============================================================================
# The parametrized_subtractor is a header-only template library that supports:
#   - 8-bit signed integers (-128 to 127)
#   - 16-bit signed integers (-32768 to 32767)
#   - 32-bit signed integers (-2147483648 to 2147483647)
# Features: Overflow/underflow detection with saturation behavior
# ============================================================================

set(TESTBENCH_PARAMETRIZED
    testbench/tb_parametrized.cpp
)

add_executable(tb_parametrized ${TESTBENCH_PARAMETRIZED})
target_link_libraries(tb_parametrized ${SYSTEMC_LIBRARIES})

set_target_properties(tb_parametrized PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testbench
)
