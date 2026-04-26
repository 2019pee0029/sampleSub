#include <systemc.h>
#include <iostream>
#include <iomanip>
#include <string>
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
        
        // Test boundary conditions, saturation, and flag validation
        test_boundary_saturation_and_flags();
        
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
            {1, 1, 0, false, false},            // Subtract equal values
            {-5, 10, -15, false, false},        // Small negative result
            {5, -10, 15, false, false},         // Small positive result
            
            // Overflow cases (result > 127)
            {127, -1, 127, true, false},        // 127 - (-1) = 128 -> SAT to 127
            {100, -50, 127, true, false},       // 100 - (-50) = 150 -> SAT to 127
            {127, 0, 127, false, false},        // Edge case: max value
            {75, -75, 127, true, false},        // 75 - (-75) = 150 -> overflow
            {60, -70, 127, true, false},        // 60 - (-70) = 130 -> overflow
            
            // Underflow cases (result < -128)
            {-128, 1, -128, false, true},       // -128 - 1 = -129 -> SAT to -128
            {-100, 50, -128, false, true},      // -100 - 50 = -150 -> SAT to -128
            {-128, 0, -128, false, false},      // Edge case: min value
            {-60, 70, -128, false, true},       // -60 - 70 = -130 -> underflow
            {-75, 75, -128, false, true},       // -75 - 75 = -150 -> underflow
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
            {0, 0, 0, false, false},
            {1000, 1000, 0, false, false},
            {-10000, -5000, -5000, false, false},
            {5000, -5000, 10000, false, false},
            
            // Overflow cases (result > 32767)
            {32767, -1, 32767, true, false},    // 32767 - (-1) = 32768 -> SAT
            {20000, -20000, 32767, true, false}, // 20000 - (-20000) = 40000 -> SAT
            {32000, -1000, 32767, true, false},  // 32000 - (-1000) = 33000 -> overflow
            {16384, -20000, 32767, true, false}, // 16384 - (-20000) = 36384 -> overflow
            
            // Underflow cases (result < -32768)
            {-32768, 1, -32768, false, true},   // -32768 - 1 = -32769 -> SAT
            {-20000, 20000, -32768, false, true}, // -20000 - 20000 = -40000 -> SAT
            {-32000, 1000, -32768, false, true}, // -32000 - 1000 = -33000 -> underflow
            {-16384, 20000, -32768, false, true}, // -16384 - 20000 = -36384 -> underflow
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
            {0, 0, 0, false, false},
            {-100000, -50000, -50000, false, false},
            {1500000, -1500000, 3000000, false, false},
            
            // Overflow cases
            {2147483647, -1, 2147483647, true, false},  // Exceeds max
            {1500000000, -700000000, 2147483647, true, false},  // Large overflow
            {2000000000, -200000000, 2147483647, true, false},  // Exceeds by small amount
            
            // Underflow cases
            {-2147483648, 1, -2147483648, false, true}, // Exceeds min
            {-1500000000, 700000000, -2147483648, false, true},  // Large underflow
            {-2000000000, 200000000, -2147483648, false, true},  // Exceeds by small amount
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
    
    void test_boundary_saturation_and_flags()
    {
        cout << "\n[TEST 4] Critical Boundary Cases, Saturation Verification & Flag Validation" << endl;
        cout << string(70, '=') << endl;
        
        test_8bit_boundary_and_flags();
        test_16bit_boundary_and_flags();
        test_32bit_boundary_and_flags();
    }
    
    void test_8bit_boundary_and_flags()
    {
        cout << "\n[8-BIT] Boundary & Saturation Tests" << endl;
        cout << string(70, '-') << endl;
        cout << "A    | B    | Result | Expected | OV | UF | Flags_OK | Sat_OK | Status" << endl;
        cout << string(70, '-') << endl;
        
        int pass_count = 0, flag_errors = 0, sat_errors = 0;
        
        struct TestCase {
            int8_t a, b, expected;
            bool exp_ov, exp_uf;
            const char* desc;
        } tests[] = {
            // Identity: a - a = 0 (should never overflow/underflow)
            {127, 127, 0, false, false, "MAX-MAX=0"},
            {-128, -128, 0, false, false, "MIN-MIN=0"},
            {42, 42, 0, false, false, "Identity"},
            
            // Zero subtraction edge cases
            {0, 127, -127, false, false, "0-MAX"},
            {0, -128, 127, true, false, "0-MIN (OV to 127)"},
            {127, 0, 127, false, false, "MAX-0"},
            {-128, 0, -128, false, false, "MIN-0"},
            
            // MIN/MAX arithmetic edge cases
            {-128, -1, -127, false, false, "MIN-(-1)"},
            {127, 1, 126, false, false, "MAX-1"},
            {1, 127, -126, false, false, "1-MAX"},
            {-1, -128, 127, true, false, "-1-MIN (OV to 127)"},
            
            // Saturation verification (should clamp exactly)
            {100, -50, 127, true, false, "Saturate to MAX"},
            {-100, 50, -128, false, true, "Saturate to MIN"},
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
            
            // Validation checks
            bool result_ok = (result == tests[i].expected);
            bool ov_ok = (ov == tests[i].exp_ov);
            bool uf_ok = (uf == tests[i].exp_uf);
            
            // Flag validation: mutual exclusivity (both can't be true)
            bool flags_exclusive = !(ov && uf);
            if (!flags_exclusive) flag_errors++;
            
            // Saturation verification: if overflow, result must be exactly MAX_VAL (127)
            bool sat_ok = true;
            if (tests[i].exp_ov && result != 127) sat_ok = false;
            if (tests[i].exp_uf && result != -128) sat_ok = false;
            if (!sat_ok) sat_errors++;
            
            bool pass = result_ok && ov_ok && uf_ok && flags_exclusive && sat_ok;
            
            cout << setw(4) << (int)tests[i].a << " | "
                 << setw(4) << (int)tests[i].b << " | "
                 << setw(6) << (int)result << " | "
                 << setw(8) << (int)tests[i].expected << " | "
                 << (ov ? "Y" : "N") << "  | "
                 << (uf ? "Y" : "N") << "  | "
                 << (flags_exclusive ? "OK" : "FAIL") << " | "
                 << (sat_ok ? "OK" : "FAIL") << " | "
                 << (pass ? "PASS" : "FAIL") << " (" << tests[i].desc << ")" << endl;
            
            if (pass) pass_count++;
        }
        
        cout << string(70, '-') << endl;
        cout << "8-bit Boundary Tests: " << pass_count << "/" << num_tests << " passed";
        if (flag_errors > 0) cout << " [FLAG ERRORS: " << flag_errors << "]";
        if (sat_errors > 0) cout << " [SATURATION ERRORS: " << sat_errors << "]";
        cout << endl;
    }
    
    void test_16bit_boundary_and_flags()
    {
        cout << "\n[16-BIT] Boundary & Saturation Tests" << endl;
        cout << string(70, '-') << endl;
        cout << "A      | B      | Result  | Expected | OV | UF | Flags_OK | Sat_OK | Status" << endl;
        cout << string(70, '-') << endl;
        
        int pass_count = 0, flag_errors = 0, sat_errors = 0;
        
        struct TestCase {
            int16_t a, b, expected;
            bool exp_ov, exp_uf;
            const char* desc;
        } tests[] = {
            // Identity tests
            {32767, 32767, 0, false, false, "MAX-MAX=0"},
            {-32768, -32768, 0, false, false, "MIN-MIN=0"},
            {5000, 5000, 0, false, false, "Identity"},
            
            // Zero subtraction
            {0, 32767, -32767, false, false, "0-MAX"},
            {0, -32768, 32767, true, false, "0-MIN (OV)"},
            {32767, 0, 32767, false, false, "MAX-0"},
            {-32768, 0, -32768, false, false, "MIN-0"},
            
            // MIN/MAX arithmetic
            {-32768, -1, -32767, false, false, "MIN-(-1)"},
            {32767, 1, 32766, false, false, "MAX-1"},
            {1, 32767, -32766, false, false, "1-MAX"},
            {-1, -32768, 32767, true, false, "-1-MIN (OV)"},
            
            // Saturation
            {20000, -20000, 32767, true, false, "Saturate to MAX"},
            {-20000, 20000, -32768, false, true, "Saturate to MIN"},
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
            
            bool result_ok = (result == tests[i].expected);
            bool ov_ok = (ov == tests[i].exp_ov);
            bool uf_ok = (uf == tests[i].exp_uf);
            
            bool flags_exclusive = !(ov && uf);
            if (!flags_exclusive) flag_errors++;
            
            bool sat_ok = true;
            if (tests[i].exp_ov && result != 32767) sat_ok = false;
            if (tests[i].exp_uf && result != -32768) sat_ok = false;
            if (!sat_ok) sat_errors++;
            
            bool pass = result_ok && ov_ok && uf_ok && flags_exclusive && sat_ok;
            
            cout << setw(6) << tests[i].a << " | "
                 << setw(6) << tests[i].b << " | "
                 << setw(7) << result << " | "
                 << setw(8) << tests[i].expected << " | "
                 << (ov ? "Y" : "N") << "  | "
                 << (uf ? "Y" : "N") << "  | "
                 << (flags_exclusive ? "OK" : "FAIL") << " | "
                 << (sat_ok ? "OK" : "FAIL") << " | "
                 << (pass ? "PASS" : "FAIL") << " (" << tests[i].desc << ")" << endl;
            
            if (pass) pass_count++;
        }
        
        cout << string(70, '-') << endl;
        cout << "16-bit Boundary Tests: " << pass_count << "/" << num_tests << " passed";
        if (flag_errors > 0) cout << " [FLAG ERRORS: " << flag_errors << "]";
        if (sat_errors > 0) cout << " [SATURATION ERRORS: " << sat_errors << "]";
        cout << endl;
    }
    
    void test_32bit_boundary_and_flags()
    {
        cout << "\n[32-BIT] Boundary & Saturation Tests" << endl;
        cout << string(70, '-') << endl;
        cout << "A         | B         | Result     | Expected   | OV | UF | Flags_OK | Sat_OK | Status" << endl;
        cout << string(70, '-') << endl;
        
        int pass_count = 0, flag_errors = 0, sat_errors = 0;
        
        struct TestCase {
            int32_t a, b, expected;
            bool exp_ov, exp_uf;
            const char* desc;
        } tests[] = {
            // Identity tests
            {2147483647, 2147483647, 0, false, false, "MAX-MAX=0"},
            {-2147483648, -2147483648, 0, false, false, "MIN-MIN=0"},
            {100000000, 100000000, 0, false, false, "Identity"},
            
            // Zero subtraction
            {0, 2147483647, -2147483647, false, false, "0-MAX"},
            {0, -2147483648, 2147483647, true, false, "0-MIN (OV)"},
            {2147483647, 0, 2147483647, false, false, "MAX-0"},
            {-2147483648, 0, -2147483648, false, false, "MIN-0"},
            
            // MIN/MAX arithmetic
            {-2147483648, -1, -2147483647, false, false, "MIN-(-1)"},
            {2147483647, 1, 2147483646, false, false, "MAX-1"},
            {1, 2147483647, -2147483646, false, false, "1-MAX"},
            {-1, -2147483648, 2147483647, true, false, "-1-MIN (OV)"},
            
            // Saturation
            {1500000000, -700000000, 2147483647, true, false, "Saturate to MAX"},
            {-1500000000, 700000000, -2147483648, false, true, "Saturate to MIN"},
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
            
            bool result_ok = (result == tests[i].expected);
            bool ov_ok = (ov == tests[i].exp_ov);
            bool uf_ok = (uf == tests[i].exp_uf);
            
            bool flags_exclusive = !(ov && uf);
            if (!flags_exclusive) flag_errors++;
            
            bool sat_ok = true;
            if (tests[i].exp_ov && result != 2147483647) sat_ok = false;
            if (tests[i].exp_uf && result != -2147483648) sat_ok = false;
            if (!sat_ok) sat_errors++;
            
            bool pass = result_ok && ov_ok && uf_ok && flags_exclusive && sat_ok;
            
            cout << setw(9) << tests[i].a << " | "
                 << setw(9) << tests[i].b << " | "
                 << setw(10) << result << " | "
                 << setw(10) << tests[i].expected << " | "
                 << (ov ? "Y" : "N") << "  | "
                 << (uf ? "Y" : "N") << "  | "
                 << (flags_exclusive ? "OK" : "FAIL") << " | "
                 << (sat_ok ? "OK" : "FAIL") << " | "
                 << (pass ? "PASS" : "FAIL") << " (" << tests[i].desc << ")" << endl;
            
            if (pass) pass_count++;
        }
        
        cout << string(70, '-') << endl;
        cout << "32-bit Boundary Tests: " << pass_count << "/" << num_tests << " passed";
        if (flag_errors > 0) cout << " [FLAG ERRORS: " << flag_errors << "]";
        if (sat_errors > 0) cout << " [SATURATION ERRORS: " << sat_errors << "]";
        cout << endl;
    }
};

int sc_main(int argc, char* argv[])
{
    testbench_parametrized tb("testbench_parametrized");
    
    sc_start();
    
    cout << "\n" << string(70, '=') << endl;
    cout << "TEST SUMMARY" << endl;
    cout << "Tests include:" << endl;
    cout << "  - Basic arithmetic operations (TEST 1-3)" << endl;
    cout << "  - Critical boundary cases & identity operations (TEST 4)" << endl;
    cout << "  - Flag mutual exclusivity validation" << endl;
    cout << "  - Saturation correctness verification" << endl;
    cout << string(70, '=') << endl;
    
    return 0;
}
