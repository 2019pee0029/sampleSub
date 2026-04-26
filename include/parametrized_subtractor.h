#ifndef PARAMETRIZED_SUBTRACTOR_H
#define PARAMETRIZED_SUBTRACTOR_H

#include <systemc.h>
#include <cstdint>
#include <limits>

// Template-based parametrized subtractor with overflow/underflow detection
// Template parameter: BitWidth (8, 16, or 32)

template <int BitWidth>
SC_MODULE(parametrized_subtractor)
{
    // Determine the appropriate data type based on BitWidth
    using DataType = typename std::conditional<
        BitWidth == 8, int8_t,
        typename std::conditional<BitWidth == 16, int16_t, int32_t>::type
    >::type;

    // Input ports
    sc_in<DataType>   a;
    sc_in<DataType>   b;
    
    // Output ports
    sc_out<DataType>  difference;      // Result (saturated if overflow/underflow)
    sc_out<bool>      overflow_flag;   // High when result exceeds max value
    sc_out<bool>      underflow_flag;  // High when result is below min value
    
    // Constructor
    SC_CTOR(parametrized_subtractor)
    {
        SC_METHOD(compute_subtraction);
        sensitive << a << b;
    }
    
    // Process: combinational logic for subtraction with overflow/underflow detection
    void compute_subtraction()
    {
        DataType a_val = a.read();
        DataType b_val = b.read();
        
        // Get the range for this data type
        constexpr DataType MIN_VAL = std::numeric_limits<DataType>::min();
        constexpr DataType MAX_VAL = std::numeric_limits<DataType>::max();
        
        // Cast to larger type to detect overflow/underflow
        int64_t result_extended = static_cast<int64_t>(a_val) - static_cast<int64_t>(b_val);
        
        // Check for overflow/underflow conditions
        bool overflow = (result_extended > MAX_VAL);
        bool underflow = (result_extended < MIN_VAL);
        
        // Saturate result if overflow/underflow detected
        DataType saturated_result;
        if (overflow)
        {
            saturated_result = MAX_VAL;
        }
        else if (underflow)
        {
            saturated_result = MIN_VAL;
        }
        else
        {
            saturated_result = static_cast<DataType>(result_extended);
        }
        
        // Write outputs
        difference.write(saturated_result);
        overflow_flag.write(overflow);
        underflow_flag.write(underflow);
    }
};

#endif // PARAMETRIZED_SUBTRACTOR_H
