# Quickstart: iDMS Foundation Overhaul

**Feature**: `001-foundation-overhaul` | **Date**: 2026-02-14

## Prerequisites

| Tool | Version | Purpose | Install |
|------|---------|---------|---------|
| avr-gcc | 7.3+ | Cross-compiler for ATmega128 | `sudo apt install gcc-avr avr-libc` |
| avr-objcopy | (bundled) | Binary conversion | Included with gcc-avr |
| avrdude | 6.3+ | Flash programming | `sudo apt install avrdude` |
| make | 4.0+ | Build system | `sudo apt install make` |
| gcc | 9.0+ | Host-side test compilation | Usually pre-installed |
| git | 2.0+ | Version control | `sudo apt install git` |
| simavr | 1.6+ | AVR simulation (optional) | `sudo apt install simavr` |

## Quick Build

```bash
# Clone and switch to the feature branch
git clone <repo-url> iDMS
cd iDMS
git checkout 001-foundation-overhaul

# Build release firmware
make all
# Output: build/main.hex (flashable binary)

# Build debug firmware (with logging enabled)
make debug
# Output: build/main_debug.hex

# Run host-side unit tests
make test
# Output: test results on stdout

# Clean all build artifacts
make clean

# Flash to ATmega128 (requires USBasp programmer)
make upload
```

## Project Layout (post-cleanup)

```
iDMS/
├── main.c              ← Entry point
├── Makefile            ← THE build system (the only one)
├── README.md           ← Project documentation
│
├── inc/app.h           ← Application header
├── src/app.c           ← Application implementation
│
├── MCAL/               ← Hardware abstraction (MCU-specific)
│   ├── ADC/            ← Analog-to-digital converter
│   ├── DIO/            ← Digital I/O
│   ├── EEPROM/         ← Internal EEPROM driver
│   ├── UART/           ← Serial communication
│   └── Delay/          ← Portable delay abstraction
│
├── Service/            ← Sensor drivers & system services
│   ├── *_config.h      ← Per-sensor configuration
│   ├── *_interface.h   ← Public API headers
│   ├── *_program.c     ← Implementations
│   ├── debug_log.*     ← UART debug logging
│   ├── data_logger.*   ← EEPROM data recording
│   ├── error_handler.* ← Centralized error management
│   ├── enhanced_sms.*  ← SMS with retry
│   └── watchdog_timer.*← Software watchdog
│
├── Config/             ← System configuration
│   ├── system_config.* ← Runtime config (EEPROM-backed)
│   ├── eeprom_map.h    ← EEPROM address map (single source of truth)
│   └── sensor_common.h ← Shared constants (VREF, etc.)
│
├── Utilities/          ← Type definitions & macros
│   ├── STD_TYPES.h
│   ├── BIT_MATH.h
│   └── errorStates.h
│
├── Test/               ← Test source files
├── testing/            ← Test scripts & reports
├── Hardware/           ← Hardware documentation
└── specs/              ← Feature specifications
```

## Key Development Commands

```bash
# Verify the build compiles cleanly
make all 2>&1 | grep -E "error|warning" | head -20

# Check EEPROM API consistency
grep -rn "eeprom_write_byte\|eeprom_read_byte\|avr/eeprom.h" Service/ src/ inc/
# Expected: no results (all EEPROM access through MCAL)

# Check for duplicate definitions
grep -rn "#define VREF" Service/ Config/
# Expected: only in Config/sensor_common.h

# Count source files (should be ~40 after cleanup, down from 51)
find . -name '*.c' -o -name '*.h' | grep -v specs | grep -v testing | wc -l

# Check for CodeCrafters references
grep -rn "CodeCrafters" . --include="*.md" --include="*.mk" --include="Makefile"
# Expected: no results
```

## Verification Checklist

After implementing each user story, verify:

- [ ] `make all` succeeds with zero errors
- [ ] `make debug` succeeds with zero errors
- [ ] `make test` succeeds (even if no tests yet)
- [ ] `make clean` removes all artifacts
- [ ] No duplicate `.c` or `.h` files implement the same module
- [ ] `grep -rn 'avr/' Service/ src/ inc/` returns no results
- [ ] All EEPROM modules include `Config/eeprom_map.h`
- [ ] `grep -rn 'CodeCrafters'` returns no results (outside specs/)
- [ ] README accurately describes the current state
- [ ] DIO_config.h PA0/PA1 set to DIO_INPUT (ADC pins)
- [ ] DIO_config.h PC0, PC2, PC3 set to DIO_OUTPUT (GSM reset, relays)
- [ ] DIO_config.h includes Port E, F, G configurations
- [ ] Hardware docs UART labels match actual code: UART0→GSM, UART1→LCD
- [ ] Wiring reference card exists at Hardware/Wiring_Reference.md
- [ ] Power rail go/no-go voltage ranges documented

## EEPROM Memory Map Reference

```
0x000 ┌──────────────────┐
      │ Phone List       │ 512 B
0x200 ├──────────────────┤
      │ System Config    │ 128 B
0x280 ├──────────────────┤
      │ Data Logger      │ 1024 B (40 entries)
0x680 ├──────────────────┤
      │ Error Log        │ 384 B (20 entries)
0x800 ├──────────────────┤
      │ Reserved         │ 2048 B
0xFFF └──────────────────┘
```
