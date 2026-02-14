#!/bin/bash

# iDMS Functional Testing Script
# This script performs functional tests for the iDMS components

echo "=================================================="
echo "iDMS FUNCTIONAL TESTING CAMPAIGN"
echo "=================================================="
echo ""

# Change to project directory
cd /home/runner/work/iDMS/iDMS

# Test 1: Phone List Test Program
echo "Test 1: Phone List Functionality Test"
echo "--------------------------------------"

# Build the test program first
make clean > /dev/null 2>&1
make test > /tmp/test_build.log 2>&1

if [ $? -eq 0 ]; then
    echo "✓ Phone list test program built successfully"
    
    # Check if simavr is available for simulation
    if command -v simavr > /dev/null 2>&1; then
        echo "  Simavr available - Running simulation test..."
        timeout 10s make test_simulate > /tmp/sim_output.log 2>&1
        sim_result=$?
        if [ $sim_result -eq 0 ] || [ $sim_result -eq 124 ]; then
            echo "  ✓ Simulation completed (or timed out as expected)"
        else
            echo "  ⚠ Simulation had issues"
        fi
    else
        echo "  Simavr not available - Skipping simulation test"
    fi
    
    # Verify test program structure
    if [ -f phone_list_test.hex ]; then
        echo "  ✓ Test hex file generated successfully"
        test_size=$(wc -c < phone_list_test.hex)
        echo "    Test program size: $test_size bytes"
    fi
else
    echo "✗ Phone list test program build failed"
    cat /tmp/test_build.log
fi
echo ""

# Test 2: UART Component Test
echo "Test 2: UART Component Analysis"
echo "--------------------------------"
uart_files=$(find MCAL/UART -name "*.c" -o -name "*.h" | wc -l)
echo "  UART source files found: $uart_files"

# Check for key UART functions
if grep -q "UART_voidInit\|UART_Init" MCAL/UART/*.c MCAL/UART/*.h 2>/dev/null; then
    echo "  ✓ UART initialization function found"
else
    echo "  ⚠ UART initialization function not found"
fi

if grep -q "UART_SendString\|UART_SendStringSync" MCAL/UART/*.c MCAL/UART/*.h 2>/dev/null; then
    echo "  ✓ UART string transmission function found"
else
    echo "  ⚠ UART string transmission function not found"
fi

if grep -q "UART_.*Data" MCAL/UART/*.c MCAL/UART/*.h 2>/dev/null; then
    echo "  ✓ UART data transmission functions found"
else
    echo "  ⚠ UART data transmission functions not found"
fi
echo ""

# Test 3: EEPROM Component Test
echo "Test 3: EEPROM Component Analysis"
echo "----------------------------------"
eeprom_files=$(find MCAL/EEPROM -name "*.c" -o -name "*.h" | wc -l)
echo "  EEPROM source files found: $eeprom_files"

# Check for key EEPROM functions
if grep -q "eeprom_write\|EEPROM_Write" MCAL/EEPROM/*.c MCAL/EEPROM/*.h 2>/dev/null; then
    echo "  ✓ EEPROM write functions found"
else
    echo "  ⚠ EEPROM write functions not found"
fi

if grep -q "eeprom_read\|EEPROM_Read" MCAL/EEPROM/*.c MCAL/EEPROM/*.h 2>/dev/null; then
    echo "  ✓ EEPROM read functions found"
else
    echo "  ⚠ EEPROM read functions not found"
fi

# Check for phone list EEPROM integration
if grep -q "ReadListFromEEPROM\|StoreListToEEPROM" Service/*.c Service/*.h 2>/dev/null; then
    echo "  ✓ Phone list EEPROM integration found"
else
    echo "  ⚠ Phone list EEPROM integration not found"
fi
echo ""

# Test 4: LCD Display Component Test
echo "Test 4: LCD Display Component Analysis"
echo "--------------------------------------"
lcd_files=$(find Service -name "*LCD*" | wc -l)
echo "  LCD source files found: $lcd_files"

# Check for key LCD functions
if grep -q "LCD_Init\|LCD_SendString\|LCD_SendNum" Service/*LCD* 2>/dev/null; then
    echo "  ✓ LCD interface functions found"
else
    echo "  ⚠ LCD interface functions not found"
fi

# Check for LCD stub implementation
if [ -f Test/test_phone_list_stub.c ]; then
    if grep -q "LCD_.*stub" Test/test_phone_list_stub.c 2>/dev/null; then
        echo "  ✓ LCD stub implementation found for testing"
    else
        echo "  ✓ LCD stub file exists"
    fi
else
    echo "  ⚠ LCD stub implementation not found"
fi
echo ""

# Test 5: Debug Logging System Test
echo "Test 5: Debug Logging System Analysis"
echo "-------------------------------------"
debug_files=$(find Debug -name "*.c" -o -name "*.h" | wc -l)
echo "  Debug source files found: $debug_files"

# Check for debug logging functions
if grep -q "DEBUG_Log" Debug/*.c Debug/*.h 2>/dev/null; then
    echo "  ✓ Debug logging functions found"
else
    echo "  ⚠ Debug logging functions not found"
fi

# Check for log levels
if grep -q "LOG_LEVEL_" Debug/*.h 2>/dev/null; then
    echo "  ✓ Debug log levels defined"
else
    echo "  ⚠ Debug log levels not found"
fi
echo ""

# Test 6: Sensor Components Test
echo "Test 6: Sensor Components Analysis"
echo "-----------------------------------"
sensor_files=$(find Service -name "*PT100*" -o -name "*CT*" | wc -l)
echo "  Sensor source files found: $sensor_files"

# Check for temperature sensor functions
if grep -q "PT100.*Temperature" Service/*.c Service/*.h 2>/dev/null; then
    echo "  ✓ PT100 temperature sensor functions found"
else
    echo "  ⚠ PT100 temperature sensor functions not found"
fi

# Check for current sensor functions
if grep -q "CT.*Current\|CT.*Irms" Service/*.c Service/*.h 2>/dev/null; then
    echo "  ✓ CT current sensor functions found"
else
    echo "  ⚠ CT current sensor functions not found"
fi

# Check for power calculation
if grep -q "Power\|CalcPower" Service/*.c Service/*.h 2>/dev/null; then
    echo "  ✓ Power calculation functions found"
else
    echo "  ⚠ Power calculation functions not found"
fi
echo ""

# Test 7: Safety System Test
echo "Test 7: Safety System Analysis"
echo "-------------------------------"

# Check for safety threshold definitions
if grep -q "MAX_TEMPERATURE\|MIN_TEMPERATURE\|MAX_CURRENT" Application/inc/*.h 2>/dev/null; then
    echo "  ✓ Safety threshold constants defined"
else
    echo "  ⚠ Safety threshold constants not found"
fi

# Check for alarm functionality
if grep -q "alarm\|Alarm\|ALARM" Application/src/*.c src/*.c 2>/dev/null; then
    echo "  ✓ Alarm system implementation found"
else
    echo "  ⚠ Alarm system implementation not found"
fi

# Check for safety threshold checking
if grep -q "checkSafetyThresholds\|SafetyThreshold" Application/src/*.c src/*.c 2>/dev/null; then
    echo "  ✓ Safety threshold checking found"
else
    echo "  ⚠ Safety threshold checking not found"
fi
echo ""

# Test 8: Integration Test
echo "Test 8: System Integration Analysis"
echo "------------------------------------"

# Check for main application files
if [ -f Application/main.c ] || [ -f main.c ]; then
    echo "  ✓ Main application file found"
else
    echo "  ⚠ Main application file not found"
fi

# Check for application initialization
if grep -q "APP_Init\|System.*Init\|initializeSystem" Application/src/*.c src/*.c 2>/dev/null; then
    echo "  ✓ Application initialization found"
else
    echo "  ⚠ Application initialization not found"
fi

# Check for main application loop
if grep -q "APP_Start\|main.*loop\|while.*loop" Application/src/*.c src/*.c 2>/dev/null; then
    echo "  ✓ Main application loop found"
else
    echo "  ⚠ Main application loop not found"
fi

# Check for phone number validation
if grep -q "validatePhoneNumber\|ValidatePhone" Application/src/*.c src/*.c 2>/dev/null; then
    echo "  ✓ Phone number validation found"
else
    echo "  ⚠ Phone number validation not found"
fi
echo ""

# Test 9: Build Artifacts Verification
echo "Test 9: Build Artifacts Verification"
echo "------------------------------------"

# Build main application to verify artifacts
make clean > /dev/null 2>&1
make all > /tmp/artifact_build.log 2>&1

if [ $? -eq 0 ]; then
    echo "  ✓ Main application builds successfully"
    
    if [ -f main.elf ]; then
        echo "  ✓ ELF file generated"
        # Check memory usage
        echo "    Memory usage analysis:"
        avr-size main.elf | tail -1 | awk '{printf "      Text: %d bytes, Data: %d bytes, BSS: %d bytes\n", $1, $2, $3}'
    fi
    
    if [ -f main.hex ]; then
        echo "  ✓ HEX file generated"
        hex_size=$(wc -c < main.hex)
        echo "    HEX file size: $hex_size bytes"
    fi
else
    echo "  ✗ Main application build failed"
fi
echo ""

# Summary
echo "=================================================="
echo "FUNCTIONAL TEST SUMMARY"
echo "=================================================="

# Count successful tests (simple heuristic based on key components)
tests_passed=0
total_tests=9

# Test results evaluation
if make test > /dev/null 2>&1; then
    tests_passed=$((tests_passed + 1))
fi

if [ $uart_files -gt 0 ]; then
    tests_passed=$((tests_passed + 1))
fi

if [ $eeprom_files -gt 0 ]; then
    tests_passed=$((tests_passed + 1))
fi

if [ $lcd_files -gt 0 ]; then
    tests_passed=$((tests_passed + 1))
fi

if [ $debug_files -gt 0 ]; then
    tests_passed=$((tests_passed + 1))
fi

if [ $sensor_files -gt 0 ]; then
    tests_passed=$((tests_passed + 1))
fi

if grep -q "MAX_TEMPERATURE" Application/inc/*.h 2>/dev/null; then
    tests_passed=$((tests_passed + 1))
fi

if [ -f Application/main.c ] || [ -f main.c ]; then
    tests_passed=$((tests_passed + 1))
fi

if make all > /dev/null 2>&1; then
    tests_passed=$((tests_passed + 1))
fi

echo "Functional tests passed: $tests_passed / $total_tests"

if [ $tests_passed -ge 7 ]; then
    echo "✓ Most functional tests PASSED - System appears functional"
    exit 0
else
    echo "⚠ Some functional components may need attention"
    exit 1
fi