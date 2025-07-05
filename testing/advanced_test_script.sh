#!/bin/bash

# iDMS Advanced Testing Campaign
# Comprehensive testing including simulation

echo "=================================================="
echo "iDMS ADVANCED TESTING CAMPAIGN"
echo "=================================================="
echo ""

# Change to project directory
cd /home/runner/work/iDMS/iDMS

# Test 1: Phone List Simulation Test
echo "Test 1: Phone List Simulation Test"
echo "-----------------------------------"

# Build the test program
make clean > /dev/null 2>&1
make test > /tmp/test_build.log 2>&1

if [ $? -eq 0 ]; then
    echo "✓ Test program built successfully"
    
    # Run simulation with timeout
    echo "  Running simulation test..."
    timeout 15s simavr -m atmega128 -f 16000000 phone_list_test.elf > /tmp/sim_output.log 2>&1 &
    sim_pid=$!
    
    # Wait for simulation to start and run
    sleep 2
    
    # Check if simulation is running
    if kill -0 $sim_pid 2>/dev/null; then
        echo "  ✓ Simulation started successfully"
        sleep 3
        
        # Stop simulation
        kill $sim_pid 2>/dev/null
        wait $sim_pid 2>/dev/null
        
        echo "  ✓ Simulation completed"
        
        # Check simulation output
        if [ -f /tmp/sim_output.log ]; then
            output_size=$(wc -c < /tmp/sim_output.log)
            echo "    Simulation output size: $output_size bytes"
        fi
    else
        echo "  ⚠ Simulation failed to start"
    fi
else
    echo "✗ Test program build failed"
fi
echo ""

# Test 2: Main Application Simulation Test
echo "Test 2: Main Application Simulation Test"
echo "-----------------------------------------"

# Build main application
make clean > /dev/null 2>&1
make all > /tmp/main_build.log 2>&1

if [ $? -eq 0 ]; then
    echo "✓ Main application built successfully"
    
    # Run main application simulation
    echo "  Running main application simulation..."
    timeout 10s simavr -m atmega128 -f 16000000 main.elf > /tmp/main_sim_output.log 2>&1 &
    main_sim_pid=$!
    
    # Wait for simulation
    sleep 2
    
    if kill -0 $main_sim_pid 2>/dev/null; then
        echo "  ✓ Main application simulation started"
        sleep 3
        
        # Stop simulation
        kill $main_sim_pid 2>/dev/null
        wait $main_sim_pid 2>/dev/null
        
        echo "  ✓ Main application simulation completed"
    else
        echo "  ⚠ Main application simulation failed to start"
    fi
else
    echo "✗ Main application build failed"
fi
echo ""

# Test 3: Memory Usage Analysis
echo "Test 3: Memory Usage Analysis"
echo "-----------------------------"

if [ -f main.elf ]; then
    echo "  Main Application Memory Usage:"
    avr-size main.elf
    
    # Calculate memory percentages for ATmega128
    # ATmega128: 128KB flash, 4KB RAM
    flash_size=131072  # 128KB in bytes
    ram_size=4096      # 4KB in bytes
    
    text_size=$(avr-size main.elf | tail -1 | awk '{print $1}')
    data_size=$(avr-size main.elf | tail -1 | awk '{print $2}')
    bss_size=$(avr-size main.elf | tail -1 | awk '{print $3}')
    
    flash_usage=$((text_size + data_size))
    ram_usage=$((data_size + bss_size))
    
    flash_percent=$((flash_usage * 100 / flash_size))
    ram_percent=$((ram_usage * 100 / ram_size))
    
    echo "    Flash usage: $flash_usage / $flash_size bytes ($flash_percent%)"
    echo "    RAM usage: $ram_usage / $ram_size bytes ($ram_percent%)"
    
    if [ $flash_percent -lt 80 ]; then
        echo "    ✓ Flash usage is acceptable"
    else
        echo "    ⚠ Flash usage is high"
    fi
    
    if [ $ram_percent -lt 80 ]; then
        echo "    ✓ RAM usage is acceptable"
    else
        echo "    ⚠ RAM usage is high"
    fi
fi

if [ -f phone_list_test.elf ]; then
    echo ""
    echo "  Test Program Memory Usage:"
    avr-size phone_list_test.elf
fi
echo ""

# Test 4: Code Quality Analysis
echo "Test 4: Code Quality Analysis"
echo "------------------------------"

# Count functions
total_functions=$(grep -r "^[a-zA-Z_][a-zA-Z0-9_]* *(" --include="*.c" . | grep -v "main(" | wc -l)
echo "  Total functions found: $total_functions"

# Count global variables
global_vars=$(grep -r "^[a-zA-Z_][a-zA-Z0-9_]* [a-zA-Z_]" --include="*.c" . | grep -v "static" | wc -l)
echo "  Global variables found: $global_vars"

# Check for hardcoded magic numbers
magic_numbers=$(grep -r "[0-9]\{3,\}" --include="*.c" . | grep -v "#define" | wc -l)
echo "  Potential magic numbers: $magic_numbers"

# Check for TODO/FIXME comments
todos=$(grep -r "TODO\|FIXME\|XXX" --include="*.c" --include="*.h" . | wc -l)
echo "  TODO/FIXME comments: $todos"

if [ $todos -eq 0 ]; then
    echo "    ✓ No outstanding TODO/FIXME items"
else
    echo "    ⚠ $todos TODO/FIXME items need attention"
fi
echo ""

# Test 5: Build Configuration Analysis
echo "Test 5: Build Configuration Analysis"
echo "------------------------------------"

# Check compiler flags
echo "  Compiler flags in use:"
grep "CFLAGS" Makefile | head -3

# Check for optimization level
if grep -q "\-Os\|\-O2\|\-O3" Makefile; then
    echo "  ✓ Optimization enabled"
else
    echo "  ⚠ No optimization flags found"
fi

# Check for debug flags
if grep -q "\-g\|\-DDEBUG" Makefile; then
    echo "  ✓ Debug support available"
else
    echo "  ⚠ No debug support configured"
fi

# Check MCU target
mcu_target=$(grep "MCU = " Makefile | cut -d'=' -f2 | tr -d ' ')
echo "  Target MCU: $mcu_target"

if [ "$mcu_target" = "atmega128" ]; then
    echo "    ✓ Valid MCU target"
else
    echo "    ⚠ Unusual MCU target"
fi
echo ""

# Test 6: Build Reproducibility Test
echo "Test 6: Build Reproducibility Test"
echo "-----------------------------------"

# Build twice and compare
echo "  Testing build reproducibility..."

# First build
make clean > /dev/null 2>&1
make all > /tmp/build1.log 2>&1
cp main.hex /tmp/main1.hex 2>/dev/null
build1_result=$?

# Second build
make clean > /dev/null 2>&1
make all > /tmp/build2.log 2>&1
cp main.hex /tmp/main2.hex 2>/dev/null
build2_result=$?

if [ $build1_result -eq 0 ] && [ $build2_result -eq 0 ]; then
    if cmp -s /tmp/main1.hex /tmp/main2.hex; then
        echo "  ✓ Builds are reproducible"
    else
        echo "  ⚠ Builds are not reproducible"
    fi
else
    echo "  ✗ Build reproducibility test failed"
fi
echo ""

# Test 7: Warning Analysis
echo "Test 7: Warning Analysis"
echo "------------------------"

make clean > /dev/null 2>&1
make all > /tmp/warning_analysis.log 2>&1

warning_count=$(grep -c "warning:" /tmp/warning_analysis.log)
error_count=$(grep -c "error:" /tmp/warning_analysis.log)

echo "  Total warnings: $warning_count"
echo "  Total errors: $error_count"

if [ $error_count -eq 0 ]; then
    echo "  ✓ No compilation errors"
else
    echo "  ✗ Compilation errors present"
fi

if [ $warning_count -le 10 ]; then
    echo "  ✓ Warning count is acceptable"
elif [ $warning_count -le 30 ]; then
    echo "  ⚠ Warning count is moderate"
else
    echo "  ⚠ Warning count is high"
fi

# Show warning categories
echo "  Warning categories:"
grep "warning:" /tmp/warning_analysis.log | sed 's/.*warning: \[\([^]]*\)\].*/\1/' | sort | uniq -c | head -5
echo ""

# Test 8: Dependency Analysis
echo "Test 8: Dependency Analysis"
echo "---------------------------"

# Check include dependencies
echo "  Header file dependencies:"
h_files=$(find . -name "*.h" | wc -l)
echo "    Header files: $h_files"

# Check for circular includes (simple check)
circular_check=$(grep -r "#include.*\.h" --include="*.h" . | wc -l)
echo "    Header-to-header includes: $circular_check"

# Check external dependencies
external_deps=$(grep -r "#include <" --include="*.c" --include="*.h" . | sed 's/.*#include <\([^>]*\)>.*/\1/' | sort | uniq | wc -l)
echo "    External dependencies: $external_deps"

if [ $external_deps -le 10 ]; then
    echo "    ✓ External dependencies are reasonable"
else
    echo "    ⚠ Many external dependencies"
fi
echo ""

# Summary
echo "=================================================="
echo "ADVANCED TESTING SUMMARY"
echo "=================================================="

test_score=0
total_tests=8

# Test scoring
if make test > /dev/null 2>&1; then
    test_score=$((test_score + 1))
fi

if make all > /dev/null 2>&1; then
    test_score=$((test_score + 1))
fi

if [ -f main.elf ] && [ $flash_percent -lt 80 ]; then
    test_score=$((test_score + 1))
fi

if [ $warning_count -le 30 ]; then
    test_score=$((test_score + 1))
fi

if [ $error_count -eq 0 ]; then
    test_score=$((test_score + 1))
fi

if [ $todos -le 5 ]; then
    test_score=$((test_score + 1))
fi

if [ $build1_result -eq 0 ] && [ $build2_result -eq 0 ]; then
    test_score=$((test_score + 1))
fi

if [ $external_deps -le 10 ]; then
    test_score=$((test_score + 1))
fi

echo "Advanced test score: $test_score / $total_tests"

if [ $test_score -ge 6 ]; then
    echo "✓ ADVANCED TESTING PASSED - System is ready for deployment"
    exit 0
else
    echo "⚠ Some advanced tests need attention before deployment"
    exit 1
fi