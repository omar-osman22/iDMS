#!/bin/bash

# iDMS Master Testing Script
# Runs the complete testing campaign

echo "=================================================="
echo "iDMS MASTER TESTING CAMPAIGN"
echo "=================================================="
echo ""

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Ensure we're in the right directory
cd "$SCRIPT_DIR/.."

echo "Running comprehensive testing campaign for iDMS..."
echo "Working directory: $(pwd)"
echo ""

# Phase 1: Compilation Testing
echo "🔨 PHASE 1: COMPILATION TESTING"
echo "================================"
if [ -f testing/compilation_test_script.sh ]; then
    chmod +x testing/compilation_test_script.sh
    testing/compilation_test_script.sh
    compilation_result=$?
else
    echo "❌ Compilation test script not found!"
    compilation_result=1
fi
echo ""

# Phase 2: Functional Testing
echo "⚙️  PHASE 2: FUNCTIONAL TESTING"
echo "==============================="
if [ -f testing/functional_test_script.sh ]; then
    chmod +x testing/functional_test_script.sh
    testing/functional_test_script.sh
    functional_result=$?
else
    echo "❌ Functional test script not found!"
    functional_result=1
fi
echo ""

# Phase 3: Advanced Testing
echo "🚀 PHASE 3: ADVANCED TESTING"
echo "============================"
if [ -f testing/advanced_test_script.sh ]; then
    chmod +x testing/advanced_test_script.sh
    testing/advanced_test_script.sh
    advanced_result=$?
else
    echo "❌ Advanced test script not found!"
    advanced_result=1
fi
echo ""

# Final Summary
echo "=================================================="
echo "🎯 FINAL TESTING SUMMARY"
echo "=================================================="

phases_passed=0
total_phases=3

if [ $compilation_result -eq 0 ]; then
    echo "✅ Phase 1: Compilation Testing - PASSED"
    phases_passed=$((phases_passed + 1))
else
    echo "❌ Phase 1: Compilation Testing - FAILED"
fi

if [ $functional_result -eq 0 ]; then
    echo "✅ Phase 2: Functional Testing - PASSED"
    phases_passed=$((phases_passed + 1))
else
    echo "❌ Phase 2: Functional Testing - FAILED"
fi

if [ $advanced_result -eq 0 ]; then
    echo "✅ Phase 3: Advanced Testing - PASSED"
    phases_passed=$((phases_passed + 1))
else
    echo "❌ Phase 3: Advanced Testing - FAILED"
fi

echo ""
echo "Overall Result: $phases_passed / $total_phases phases passed"

if [ $phases_passed -eq $total_phases ]; then
    echo ""
    echo "🎉 ALL TESTING PHASES COMPLETED SUCCESSFULLY!"
    echo "📋 iDMS system is ready for deployment"
    echo "📄 See testing/TEST_REPORT.md for detailed results"
    echo ""
    echo "Next steps:"
    echo "  1. Review the test report"
    echo "  2. Address any warnings if needed"
    echo "  3. Deploy to target hardware"
    echo "  4. Perform hardware-in-the-loop testing"
    exit 0
else
    echo ""
    echo "⚠️  SOME TESTING PHASES FAILED"
    echo "📋 Please review failed tests before deployment"
    echo "📄 See individual test logs for details"
    exit 1
fi