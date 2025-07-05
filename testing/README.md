# iDMS Testing Framework

This directory contains the comprehensive testing framework for the iDMS (Intelligent Device Monitoring System) project.

## Overview

The testing framework consists of three phases:
1. **Compilation Testing** - Verifies that all code compiles successfully
2. **Functional Testing** - Tests individual components and system integration
3. **Advanced Testing** - Performs simulation, memory analysis, and quality checks

## Quick Start

To run all tests:
```bash
./run_all_tests.sh
```

To run individual test phases:
```bash
./compilation_test_script.sh    # Phase 1: Compilation
./functional_test_script.sh     # Phase 2: Functional
./advanced_test_script.sh       # Phase 3: Advanced
```

## Prerequisites

### Required Tools
- **avr-gcc** - AVR GCC compiler toolchain
- **avr-libc** - AVR C library
- **avrdude** - AVR programmer
- **simavr** - AVR simulator (for simulation testing)
- **make** - GNU Make build system

### Installation (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y gcc-avr avr-libc avrdude simavr make
```

## Test Phases

### Phase 1: Compilation Testing
Tests the build system and compilation process:
- Clean build verification
- Main application compilation
- Debug build compilation  
- Test program compilation
- Memory usage analysis
- Warning/error reporting

**Expected Duration**: ~2 minutes

### Phase 2: Functional Testing
Verifies component functionality and integration:
- Phone list management
- UART communication
- EEPROM storage operations
- LCD display interface
- Debug logging system
- Sensor components (PT100, CT)
- Safety threshold monitoring
- System integration

**Expected Duration**: ~3 minutes

### Phase 3: Advanced Testing
Performs comprehensive system validation:
- Simulation testing (using simavr)
- Memory usage optimization
- Code quality analysis
- Build reproducibility
- Dependency analysis
- Warning categorization

**Expected Duration**: ~4 minutes

## Test Results

Test results are logged to temporary files and summarized in the console output. A comprehensive test report is generated in `TEST_REPORT.md`.

### Success Criteria
- **All phases must pass** for deployment readiness
- **Memory usage** should be under 80% for both flash and RAM
- **No compilation errors** are acceptable
- **Simulation tests** must run without crashes

### Warning Levels
- **< 10 warnings**: Excellent
- **10-30 warnings**: Acceptable
- **> 30 warnings**: Needs attention

## Target Hardware

The tests are configured for:
- **MCU**: ATmega128
- **Clock**: 16 MHz
- **Flash**: 128 KB
- **RAM**: 4 KB

## Customization

### Modifying Test Parameters
Edit the individual test scripts to adjust:
- Simulation timeout values
- Memory usage thresholds
- Warning count limits
- Test coverage requirements

### Adding New Tests
1. Add test logic to appropriate phase script
2. Update test counters in summary sections
3. Document new tests in this README

## Troubleshooting

### Common Issues

**avr-gcc not found**
```bash
sudo apt install gcc-avr avr-libc
```

**simavr not found**
```bash
sudo apt install simavr
```

**Build fails with missing headers**
- Check include paths in Makefile
- Verify all source files are present

**Simulation timeout**
- Increase timeout values in test scripts
- Check if simulation actually starts

### Debug Mode
To run tests with verbose output, edit the scripts and remove redirections to `/dev/null`.

## Continuous Integration

For CI/CD integration, use the master test runner:
```bash
# In CI pipeline
./testing/run_all_tests.sh
if [ $? -eq 0 ]; then
    echo "Tests passed - ready for deployment"
else
    echo "Tests failed - deployment blocked"
    exit 1
fi
```

## File Structure

```
testing/
├── README.md                    # This file
├── run_all_tests.sh            # Master test runner
├── compilation_test_script.sh   # Phase 1 tests
├── functional_test_script.sh    # Phase 2 tests  
├── advanced_test_script.sh     # Phase 3 tests
├── TEST_REPORT.md              # Generated test report
└── .gitignore                  # Excludes temporary files
```

## Contributing

When adding new functionality to the iDMS system:
1. Ensure all existing tests still pass
2. Add appropriate tests for new functionality
3. Update test documentation
4. Verify memory usage remains acceptable

## Support

For issues with the testing framework, check:
1. Tool installation and versions
2. Build environment setup
3. Test script permissions
4. Available system resources

---

*Last updated: December 2024*