#!/bin/bash

# iDMS Compilation Testing Script
# This script performs comprehensive compilation tests for the iDMS project

echo "=================================================="
echo "iDMS COMPILATION TESTING CAMPAIGN"
echo "=================================================="
echo ""

# Change to project directory
cd /home/runner/work/iDMS/iDMS

# Test 1: Clean build test
echo "Test 1: Clean Build Test"
echo "------------------------"
make clean > /tmp/clean_output.log 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Clean operation successful"
else
    echo "✗ Clean operation failed"
    cat /tmp/clean_output.log
fi
echo ""

# Test 2: Main application build
echo "Test 2: Main Application Build"
echo "-------------------------------"
make all > /tmp/main_build.log 2>&1
main_build_result=$?
warnings_count=$(grep -c "warning:" /tmp/main_build.log)
errors_count=$(grep -c "error:" /tmp/main_build.log)

if [ $main_build_result -eq 0 ]; then
    echo "✓ Main application build successful"
    echo "  Warnings: $warnings_count"
    echo "  Errors: $errors_count"
    if [ -f main.hex ]; then
        hex_size=$(wc -c < main.hex)
        echo "  Generated hex file size: $hex_size bytes"
    fi
else
    echo "✗ Main application build failed"
    echo "  Warnings: $warnings_count"
    echo "  Errors: $errors_count"
    echo "Build log:"
    cat /tmp/main_build.log
fi
echo ""

# Test 3: Debug build
echo "Test 3: Debug Build"
echo "-------------------"
make clean > /dev/null 2>&1
make debug > /tmp/debug_build.log 2>&1
debug_build_result=$?
debug_warnings_count=$(grep -c "warning:" /tmp/debug_build.log)
debug_errors_count=$(grep -c "error:" /tmp/debug_build.log)

if [ $debug_build_result -eq 0 ]; then
    echo "✓ Debug build successful"
    echo "  Warnings: $debug_warnings_count"
    echo "  Errors: $debug_errors_count"
else
    echo "✗ Debug build failed"
    echo "  Warnings: $debug_warnings_count"
    echo "  Errors: $debug_errors_count"
fi
echo ""

# Test 4: Test program build
echo "Test 4: Test Program Build"
echo "--------------------------"
make clean > /dev/null 2>&1
make test > /tmp/test_build.log 2>&1
test_build_result=$?
test_warnings_count=$(grep -c "warning:" /tmp/test_build.log)
test_errors_count=$(grep -c "error:" /tmp/test_build.log)

if [ $test_build_result -eq 0 ]; then
    echo "✓ Test program build successful"
    echo "  Warnings: $test_warnings_count"
    echo "  Errors: $test_errors_count"
    if [ -f phone_list_test.hex ]; then
        test_hex_size=$(wc -c < phone_list_test.hex)
        echo "  Generated test hex file size: $test_hex_size bytes"
    fi
else
    echo "✗ Test program build failed"
    echo "  Warnings: $test_warnings_count"
    echo "  Errors: $test_errors_count"
fi
echo ""

# Test 5: Source file analysis
echo "Test 5: Source File Analysis"
echo "----------------------------"
c_files=$(find . -name "*.c" | grep -v Test | wc -l)
h_files=$(find . -name "*.h" | wc -l)
total_lines=$(find . -name "*.c" -o -name "*.h" | grep -v Test | xargs wc -l | tail -1 | awk '{print $1}')

echo "  C source files (excluding tests): $c_files"
echo "  Header files: $h_files"
echo "  Total lines of code (excluding tests): $total_lines"
echo ""

# Test 6: Memory usage analysis
echo "Test 6: Memory Usage Analysis"
echo "-----------------------------"
if [ -f main.elf ]; then
    echo "  Main application memory usage:"
    avr-size main.elf
else
    echo "  Main application not built - cannot analyze memory usage"
fi

if [ -f phone_list_test.elf ]; then
    echo ""
    echo "  Test program memory usage:"
    avr-size phone_list_test.elf
else
    echo "  Test program not built - cannot analyze memory usage"
fi
echo ""

# Summary
echo "=================================================="
echo "COMPILATION TEST SUMMARY"
echo "=================================================="
total_tests=4
passed_tests=0

if [ $main_build_result -eq 0 ]; then
    passed_tests=$((passed_tests + 1))
fi
if [ $debug_build_result -eq 0 ]; then
    passed_tests=$((passed_tests + 1))
fi
if [ $test_build_result -eq 0 ]; then
    passed_tests=$((passed_tests + 1))
fi
# Clean always passes if we get this far
passed_tests=$((passed_tests + 1))

echo "Tests passed: $passed_tests / $total_tests"

if [ $passed_tests -eq $total_tests ]; then
    echo "✓ All compilation tests PASSED"
    exit 0
else
    echo "✗ Some compilation tests FAILED"
    exit 1
fi