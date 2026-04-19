#include <systemc.h>
#include <iostream>
#include "../include/subtractor.h"

using namespace std;

SC_MODULE(testbench)
{
    sc_signal<int> sig_a, sig_b, sig_difference;
    subtractor* sub_inst;
    
    SC_CTOR(testbench)
    {
        // Instantiate subtractor module
        sub_inst = new subtractor("subtractor");
        
        // Bind signals to ports
        sub_inst->a(sig_a);
        sub_inst->b(sig_b);
        sub_inst->difference(sig_difference);
        
        // Create thread for test vectors
        SC_THREAD(generate_test_vectors);
    }
    
    void generate_test_vectors()
    {
        cout << "\n=== Subtractor Test Bench ===" << endl;
        cout << "Format: A - B = Result [Expected] [PASS/FAIL]\n" << endl;
        
        int test_count = 0;
        int pass_count = 0;
        
        // Test vectors: {a, b, expected_difference}
        int tests[][3] = {
            {10, 5, 5},           // Positive - Positive
            {-10, -5, -5},        // Negative - Negative
            {10, -5, 15},         // Positive - Negative
            {-10, 5, -15},        // Negative - Positive
            {0, 0, 0},            // Zero - Zero
            {100, 50, 50},        // Larger differences
            {-50, -100, 50},      // Smaller - Larger (negative)
            {42, 42, 0},          // Same values
            {1, -1, 2},           // Small differences
            {-100, 100, -200},    // Large difference
        };
        
        int num_tests = sizeof(tests) / sizeof(tests[0]);
        
        for (int i = 0; i < num_tests; i++)
        {
            int a_val = tests[i][0];
            int b_val = tests[i][1];
            int expected = tests[i][2];
            
            // Set inputs
            sig_a.write(a_val);
            sig_b.write(b_val);
            
            // Wait for combinational logic to settle
            wait(1, SC_NS);
            
            int result = sig_difference.read();
            bool pass = (result == expected);
            
            cout << a_val << " - " << b_val << " = " << result 
                 << " [" << expected << "] "
                 << (pass ? "[PASS]" : "[FAIL]") << endl;
            
            test_count++;
            if (pass) pass_count++;
        }
        
        cout << "\n=== Test Summary ===" << endl;
        cout << "Total Tests: " << test_count << endl;
        cout << "Passed: " << pass_count << endl;
        cout << "Failed: " << (test_count - pass_count) << endl;
        cout << "Success Rate: " << (100 * pass_count / test_count) << "%" << endl;
        
        sc_stop();
    }
};

int sc_main(int argc, char* argv[])
{
    testbench tb("testbench");
    
    sc_start();
    
    return 0;
}
