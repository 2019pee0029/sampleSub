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
    
    // Constructor
    SC_CTOR(subtractor)
    {
        SC_METHOD(compute_difference);
        sensitive << a << b;
    }
    
    // Process: combinational logic
    void compute_difference()
    {
        difference.write(a.read() - b.read());
    }
};

#endif // SUBTRACTOR_H
