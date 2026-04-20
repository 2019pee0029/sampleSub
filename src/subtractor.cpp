#include "subtractor.h"

// Implementation of the compute_difference method
void subtractor::compute_difference()
{
    difference.write(a.read() - b.read());
}
