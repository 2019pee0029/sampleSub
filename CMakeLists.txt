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

# Testbench files
set(TESTBENCH
    testbench/tb_subtractor.cpp
    src/subtractor.cpp
)

# Create the testbench executable
add_executable(tb_subtractor ${TESTBENCH})

# Link the SystemC library
target_link_libraries(tb_subtractor ${SYSTEMC_LIBRARIES})

# Set output directory
set_target_properties(tb_subtractor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testbench
)
