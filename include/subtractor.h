#ifndef SUBTRACTOR_H
#define SUBTRACTOR_H

#include <systemc.h>

SC_MODULE(subtractor)
{
    // Input ports
    sc_in<int>  a;
    sc_in<int>  b;
    
    // Output port
    sc_out<int> difference;
    
    // Constructor - must be inline for SC_CTOR macro
    SC_CTOR(subtractor)
    {
        SC_METHOD(compute_difference);
        sensitive << a << b;
    }
    
    // Process: combinational logic - declared here, implemented in cpp
    void compute_difference();
};

#endif // SUBTRACTOR_H
