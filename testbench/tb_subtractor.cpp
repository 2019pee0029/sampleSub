#include <systemc.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "../include/subtractor.h"

using namespace std;

// Parametrized testbench for subtractor with different test scenarios
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
        cout << "\n" << string(70, '=') << endl;
        cout << "Parametrized Subtractor Test Bench" << endl;
        cout << string(70, '=') << endl;
        
        // Run different test scenarios
        test_basic_operations();
        test_boundary_conditions();
        test_edge_cases();
        
        cout << "\n" << string(70, '=') << endl;
        cout << "All parametrized tests completed" << endl;
        cout << string(70, '=') << endl;
        
        sc_stop();
    }
    
    void test_basic_operations()
    {
        cout << "\n[TEST 1] Basic Operations" << endl;
        cout << string(70, '-') << endl;
        cout << "A     | B     | Result | Expected | Status" << endl;
        cout << string(70, '-') << endl;
        
        // Test vectors: {a, b, expected_difference}
        vector<vector<int>> tests = {
            {10, 5, 5},           // Positive - Positive
            {-10, -5, -5},        // Negative - Negative
            {10, -5, 15},         // Positive - Negative
            {-10, 5, -15},        // Negative - Positive
            {0, 0, 0},            // Zero - Zero
        };
        
        int pass_count = run_test_scenario(tests);
        
        cout << "\nTest 1 Summary: " << pass_count << "/" << tests.size() 
             << " passed" << endl;
    }
    
    void test_boundary_conditions()
    {
        cout << "\n[TEST 2] Boundary Conditions" << endl;
        cout << string(70, '-') << endl;
        cout << "A     | B     | Result | Expected | Status" << endl;
        cout << string(70, '-') << endl;
        
        vector<vector<int>> tests = {
            {100, 50, 50},        // Larger differences
            {-50, -100, 50},      // Smaller - Larger (negative)
            {42, 42, 0},          // Same values
            {1, -1, 2},           // Small differences
            {-100, 100, -200},    // Large difference
        };
        
        int pass_count = run_test_scenario(tests);
        
        cout << "\nTest 2 Summary: " << pass_count << "/" << tests.size() 
             << " passed" << endl;
    }
    
    void test_edge_cases()
    {
        cout << "\n[TEST 3] Edge Cases" << endl;
        cout << string(70, '-') << endl;
        cout << "A     | B     | Result | Expected | Status" << endl;
        cout << string(70, '-') << endl;
        
        vector<vector<int>> tests = {
            {0, 0, 0},            // Zero cases
            {1, 0, 1},            // Subtract zero
            {0, 1, -1},           // Zero minus one
            {-1, -1, 0},          // Negative edge case
            {1000, -1000, 2000},  // Large values
        };
        
        int pass_count = run_test_scenario(tests);
        
        cout << "\nTest 3 Summary: " << pass_count << "/" << tests.size() 
             << " passed" << endl;
    }
    
    int run_test_scenario(const vector<vector<int>>& tests)
    {
        int pass_count = 0;
        
        for (size_t i = 0; i < tests.size(); i++)
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
            
            cout << setw(5) << a_val << " | " << setw(5) << b_val << " | " 
                 << setw(6) << result << " | " << setw(8) << expected << " | "
                 << (pass ? "[PASS]" : "[FAIL]") << endl;
            
            if (pass) pass_count++;
        }
        
        return pass_count;
    }
};

int sc_main(int argc, char* argv[])
{
    testbench tb("testbench");
    
    sc_start();
    
    return 0;
}
