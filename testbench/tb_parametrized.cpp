#include <systemc.h>
#include <iostream>
#include <iomanip>
#include "../include/parametrized_subtractor.h"

using namespace std;

// Testbench for parametrized subtractor with overflow/underflow detection
SC_MODULE(testbench_parametrized)
{
    // 8-bit subtractor
    parametrized_subtractor<8>* sub_8bit;
    sc_signal<int8_t>  sig_a_8, sig_b_8, sig_diff_8;
    sc_signal<bool>    sig_ov_8, sig_uf_8;
    
    // 16-bit subtractor
    parametrized_subtractor<16>* sub_16bit;
    sc_signal<int16_t> sig_a_16, sig_b_16, sig_diff_16;
    sc_signal<bool>    sig_ov_16, sig_uf_16;
    
    // 32-bit subtractor
    parametrized_subtractor<32>* sub_32bit;
    sc_signal<int32_t> sig_a_32, sig_b_32, sig_diff_32;
    sc_signal<bool>    sig_ov_32, sig_uf_32;
    
    SC_CTOR(testbench_parametrized)
    {
        // Instantiate 8-bit subtractor
        sub_8bit = new parametrized_subtractor<8>("sub_8bit");
        sub_8bit->a(sig_a_8);
        sub_8bit->b(sig_b_8);
        sub_8bit->difference(sig_diff_8);
        sub_8bit->overflow_flag(sig_ov_8);
        sub_8bit->underflow_flag(sig_uf_8);
        
        // Instantiate 16-bit subtractor
        sub_16bit = new parametrized_subtractor<16>("sub_16bit");
        sub_16bit->a(sig_a_16);
        sub_16bit->b(sig_b_16);
        sub_16bit->difference(sig_diff_16);
        sub_16bit->overflow_flag(sig_ov_16);
        sub_16bit->underflow_flag(sig_uf_16);
        
        // Instantiate 32-bit subtractor
        sub_32bit = new parametrized_subtractor<32>("sub_32bit");
        sub_32bit->a(sig_a_32);
        sub_32bit->b(sig_b_32);
        sub_32bit->difference(sig_diff_32);
        sub_32bit->overflow_flag(sig_ov_32);
        sub_32bit->underflow_flag(sig_uf_32);
        
        // Create thread for test vectors
        SC_THREAD(generate_test_vectors);
    }
    
    void generate_test_vectors()
    {
        cout << "\n" << string(70, '=') << endl;
        cout << "Parametrized Subtractor with Overflow/Underflow Detection" << endl;
        cout << string(70, '=') << endl;
        
        // Test 8-bit subtractor
        test_8bit_subtractor();
        
        // Test 16-bit subtractor
        test_16bit_subtractor();
        
        // Test 32-bit subtractor
        test_32bit_subtractor();
        
        cout << "\n" << string(70, '=') << endl;
        cout << "All tests completed" << endl;
        cout << string(70, '=') << endl;
        
        sc_stop();
    }
    
    void test_8bit_subtractor()
    {
        cout << "\n[TEST 1] 8-bit Subtractor (Range: -128 to 127)" << endl;
        cout << string(70, '-') << endl;
        cout << "A    | B    | Result | Expected | OV | UF | Status" << endl;
        cout << string(70, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        // Test cases: {a, b, expected_result, expected_overflow, expected_underflow}
        struct TestCase {
            int8_t a, b, expected;
            bool exp_ov, exp_uf;
        } tests[] = {
            // Normal cases (no overflow/underflow)
            {10, 5, 5, false, false},           // Normal positive
            {-10, -5, -5, false, false},        // Normal negative
            {0, 0, 0, false, false},            // Zero result
            {50, 25, 25, false, false},         // Larger values
            
            // Overflow cases (result > 127)
            {127, -1, 127, true, false},        // 127 - (-1) = 128 -> SAT to 127
            {100, -50, 127, true, false},       // 100 - (-50) = 150 -> SAT to 127
            {127, 0, 127, false, false},        // Edge case: max value
            
            // Underflow cases (result < -128)
            {-128, 1, -128, false, true},       // -128 - 1 = -129 -> SAT to -128
            {-100, 50, -128, false, true},      // -100 - 50 = -150 -> SAT to -128
            {-128, 0, -128, false, false},      // Edge case: min value
        };
        
        int num_tests = sizeof(tests) / sizeof(tests[0]);
        
        for (int i = 0; i < num_tests; i++)
        {
            sig_a_8.write(tests[i].a);
            sig_b_8.write(tests[i].b);
            wait(1, SC_NS);
            
            int8_t result = sig_diff_8.read();
            bool ov = sig_ov_8.read();
            bool uf = sig_uf_8.read();
            
            bool result_match = (result == tests[i].expected);
            bool ov_match = (ov == tests[i].exp_ov);
            bool uf_match = (uf == tests[i].exp_uf);
            bool pass = result_match && ov_match && uf_match;
            
            cout << setw(4) << (int)tests[i].a << " | "
                 << setw(4) << (int)tests[i].b << " | "
                 << setw(6) << (int)result << " | "
                 << setw(8) << (int)tests[i].expected << " | "
                 << (ov ? "Y" : "N") << "  | "
                 << (uf ? "Y" : "N") << "  | "
                 << (pass ? "PASS" : "FAIL") << endl;
            
            test_count++;
            if (pass) pass_count++;
        }
        
        cout << string(70, '-') << endl;
        cout << "8-bit Tests: " << pass_count << "/" << test_count << " passed" << endl;
    }
    
    void test_16bit_subtractor()
    {
        cout << "\n[TEST 2] 16-bit Subtractor (Range: -32768 to 32767)" << endl;
        cout << string(70, '-') << endl;
        cout << "A      | B      | Result  | Expected | OV | UF | Status" << endl;
        cout << string(70, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int16_t a, b, expected;
            bool exp_ov, exp_uf;
        } tests[] = {
            // Normal cases
            {100, 50, 50, false, false},
            {-1000, -500, -500, false, false},
            {32767, 100, 32667, false, false},
            
            // Overflow cases (result > 32767)
            {32767, -1, 32767, true, false},    // 32767 - (-1) = 32768 -> SAT
            {20000, -20000, 32767, true, false}, // 20000 - (-20000) = 40000 -> SAT
            
            // Underflow cases (result < -32768)
            {-32768, 1, -32768, false, true},   // -32768 - 1 = -32769 -> SAT
            {-20000, 20000, -32768, false, true}, // -20000 - 20000 = -40000 -> SAT
        };
        
        int num_tests = sizeof(tests) / sizeof(tests[0]);
        
        for (int i = 0; i < num_tests; i++)
        {
            sig_a_16.write(tests[i].a);
            sig_b_16.write(tests[i].b);
            wait(1, SC_NS);
            
            int16_t result = sig_diff_16.read();
            bool ov = sig_ov_16.read();
            bool uf = sig_uf_16.read();
            
            bool result_match = (result == tests[i].expected);
            bool ov_match = (ov == tests[i].exp_ov);
            bool uf_match = (uf == tests[i].exp_uf);
            bool pass = result_match && ov_match && uf_match;
            
            cout << setw(6) << tests[i].a << " | "
                 << setw(6) << tests[i].b << " | "
                 << setw(7) << result << " | "
                 << setw(8) << tests[i].expected << " | "
                 << (ov ? "Y" : "N") << "  | "
                 << (uf ? "Y" : "N") << "  | "
                 << (pass ? "PASS" : "FAIL") << endl;
            
            test_count++;
            if (pass) pass_count++;
        }
        
        cout << string(70, '-') << endl;
        cout << "16-bit Tests: " << pass_count << "/" << test_count << " passed" << endl;
    }
    
    void test_32bit_subtractor()
    {
        cout << "\n[TEST 3] 32-bit Subtractor (Range: -2147483648 to 2147483647)" << endl;
        cout << string(70, '-') << endl;
        cout << "A         | B         | Result     | Expected   | OV | UF | Status" << endl;
        cout << string(70, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int32_t a, b, expected;
            bool exp_ov, exp_uf;
        } tests[] = {
            // Normal cases
            {1000000, 500000, 500000, false, false},
            {-2000000, -1000000, -1000000, false, false},
            {2147483647, 1000000, 2146483647, false, false},  // Edge: near max
            
            // Overflow case
            {2147483647, -1, 2147483647, true, false},  // Exceeds max
            
            // Underflow case
            {-2147483648, 1, -2147483648, false, true}, // Exceeds min
        };
        
        int num_tests = sizeof(tests) / sizeof(tests[0]);
        
        for (int i = 0; i < num_tests; i++)
        {
            sig_a_32.write(tests[i].a);
            sig_b_32.write(tests[i].b);
            wait(1, SC_NS);
            
            int32_t result = sig_diff_32.read();
            bool ov = sig_ov_32.read();
            bool uf = sig_uf_32.read();
            
            bool result_match = (result == tests[i].expected);
            bool ov_match = (ov == tests[i].exp_ov);
            bool uf_match = (uf == tests[i].exp_uf);
            bool pass = result_match && ov_match && uf_match;
            
            cout << setw(9) << tests[i].a << " | "
                 << setw(9) << tests[i].b << " | "
                 << setw(10) << result << " | "
                 << setw(10) << tests[i].expected << " | "
                 << (ov ? "Y" : "N") << "  | "
                 << (uf ? "Y" : "N") << "  | "
                 << (pass ? "PASS" : "FAIL") << endl;
            
            test_count++;
            if (pass) pass_count++;
        }
        
        cout << string(70, '-') << endl;
        cout << "32-bit Tests: " << pass_count << "/" << test_count << " passed" << endl;
    }
};

int sc_main(int argc, char* argv[])
{
    testbench_parametrized tb("testbench_parametrized");
    
    sc_start();
    
    return 0;
}
