# Industrial Data Monitoring System (iDMS)

Firmware for an ATmega128-based industrial monitoring unit that measures temperature (PT100 RTD) and current (CT sensor), displays readings on a TopWay LCD, and sends SMS alerts via a SIM800L GSM module.

---

## Table of Contents

1. [Features](#features)
2. [Hardware Overview](#hardware-overview)
3. [Repository Structure](#repository-structure)
4. [Build Instructions](#build-instructions)
5. [EEPROM Memory Map](#eeprom-memory-map)
6. [API Reference](#api-reference)
7. [Error Codes](#error-codes)
8. [Testing](#testing)
9. [MCU Migration](#mcu-migration)

---

## Features

- Real-time PT100 temperature monitoring (10-bit ADC, IIR-filtered)
- CT current monitoring with RMS calculation and derived power
- SMS alarm notifications via SIM800L GSM module (UART0)
- TopWay HMI LCD display (UART1)
- Phone number list management (stored in EEPROM)
- Data logger with circular buffer (EEPROM-backed)
- Error handler with persistent error log
- Software watchdog monitoring critical tasks
- System configuration persistence (EEPROM)
- Clean MCAL abstraction layer (migration-ready)

---

## Hardware Overview

| Component | Part | MCU Interface |
|-----------|------|---------------|
| MCU | ATmega128 @ 16 MHz | — |
| Temperature sensor | PT100 RTD + conditioning | PA0 (ADC ch0) |
| Current sensor | CT + burden resistor | PA1 (ADC ch1) |
| LCD display | TopWay HMI | **UART1** (PE0/PE1), J4 |
| GSM module | SIM800L | **UART0** (PD0/PD1), J5 |
| ISP programmer | USBasp | J8 (2×3 SPI header) |

> **UART note**: UART0 (PD0/PD1) → GSM; UART1 (PE0/PE1) → LCD.
> See [Hardware/Wiring_Reference.md](Hardware/Wiring_Reference.md) for the full pin map, connector pinouts, power-rail go/no-go procedure, and ISP instructions.

---

## Repository Structure

```
iDMS/
├── main.c                      # Application entry point
├── Makefile                    # Unified build system (all/debug/test/clean)
├── inc/
│   └── app.h                   # Application-wide types and extern declarations
├── src/
│   └── app.c                   # App logic (filtering, alarms, phone mgmt)
├── MCAL/                       # Microcontroller Abstraction Layer
│   ├── ADC/                    # ADC driver (ATmega128)
│   ├── Delay/                  # Portable delay shim (MCAL_DelayMs/MCAL_DelayUs)
│   ├── DIO/                    # Digital I/O driver + pin direction config
│   ├── EEPROM/                 # Unified EEPROM API
│   └── UART/                   # UART driver (UART0 + UART1)
├── Service/                    # Hardware-independent service layer
│   ├── CTcurrent_*.c/h         # Current sensor service
│   ├── PT100_*.c/h             # Temperature sensor service
│   ├── SIM_prog.c/h            # SIM800L GSM AT-command driver
│   ├── TopWayLCD_*.c/h         # TopWay LCD driver
│   ├── PhoneList.c/h           # Phone list management (EEPROM-backed)
│   ├── data_logger.c/h         # Data logging service
│   ├── error_handler.c/h       # Centralised error handling
│   ├── enhanced_sms.c/h        # SMS retry/formatting service
│   ├── watchdog_timer.c/h      # Software watchdog
│   └── debug_log.c/h           # Debug logging (disabled in release)
├── Config/
│   ├── eeprom_map.h            # EEPROM region macros + overlap guards
│   ├── sensor_common.h         # Shared VREF, ADC_RESOLUTION constants
│   └── system_config.c/h       # System configuration persistence
├── Utilities/
│   ├── STD_TYPES.h             # Portable type aliases (u8, u16, f32, ...)
│   ├── BIT_MATH.h              # Bit manipulation macros
│   └── errorStates.h           # Return code enum (ES_OK, ES_NOK, ...)
├── Hardware/
│   ├── Wiring_Reference.md     # Pin map, connectors, ISP, power rails
│   ├── Hardware_Integration.md
│   ├── PCB_Schematic.md
│   ├── BOM.md
│   └── Safety_Guidelines.md
├── docs/
│   └── MCU_Migration_Report.md # STM32F411 vs ESP32-S3 comparison
├── Test/
│   └── test_phone_list_stub.c  # Host-side placeholder test
└── specs/                      # Feature specification documents
```

---

## Build Instructions

### Prerequisites

```bash
# Install AVR toolchain (Ubuntu/Debian)
sudo apt install gcc-avr avr-libc binutils-avr avrdude

# Verify
avr-gcc --version
```

### Targets

| Target | Command | Description |
|--------|---------|-------------|
| Release firmware | `make all` | Produces `build/main.hex` with `-Os` |
| Debug firmware | `make debug` | Adds `-DDEBUG_ENABLE -g -O0` |
| Host tests | `make test` | Compiles host-side stubs with `gcc -DTARGET_HOST` |
| Clean | `make clean` | Removes the `build/` directory |
| Upload | `make upload` | Flashes `build/main.hex` via USBasp |

### Quick Start

```bash
git clone <repo-url> && cd iDMS

make clean && make all

# Flash via USBasp connected to J8
avrdude -c usbasp -p m128 -U flash:w:build/main.hex:i
```

See [Hardware/Wiring_Reference.md §3](Hardware/Wiring_Reference.md#3-isp-programming-procedure) for the full ISP procedure and fuse bit settings.

---

## EEPROM Memory Map

Defined in [Config/eeprom_map.h](Config/eeprom_map.h).  
`_Static_assert` guards prevent region overlaps at compile time.

| Region | Base | Size | Content |
|--------|------|------|---------|
| Phone list | `0x000` | 512 B | Up to 20 phone numbers (11 digits + null) |
| System config | `0x200` | 128 B | Thresholds, filter coefficients, version |
| Data log | `0x280` | 1024 B | Circular sensor-reading log (timestamped) |
| Error log | `0x680` | 384 B | Error records with code, timestamp, description |
| Reserved | `0x800` | to end | Future use |

ATmega128 EEPROM total: 4096 B (0x000–0xFFF).

---

## API Reference

### Core Application (`src/app.c`)

```c
void APP_Init(void);                    // Initialise all subsystems
void APP_Start(void);                   // Main loop (does not return)
f32  APP_GetFilteredTemperature(void);  // IIR-filtered temperature (°C)
f32  APP_GetFilteredCurrent(void);      // IIR-filtered current (A)
f32  APP_GetCalculatedPower(void);      // Derived power = V × I (W)
u8   APP_IsSystemInAlarm(void);         // 1 if any threshold exceeded
```

### Configuration (`Config/system_config.c`)

```c
u8   CONFIG_Init(void);         // Load config from EEPROM
u8   CONFIG_Save(void);         // Persist config to EEPROM
void CONFIG_LoadDefaults(void); // Reset to safe defaults
void CONFIG_PrintCurrent(void); // Print current config (debug)
```

### Data Logger (`Service/data_logger.c`)

```c
void DATALOG_Init(void);
void DATALOG_LogReading(f32 temp, f32 current, f32 power);
void DATALOG_Print(void);
void DATALOG_Clear(void);
```

### Error Handler (`Service/error_handler.c`)

```c
void ERROR_Init(void);
void ERROR_Report(u16 code, const char* description);
void ERROR_PrintLog(void);
void ERROR_ClearLog(void);
u8   ERROR_HasCritical(void);
```

### EEPROM (`MCAL/EEPROM/INTERNAL_EEPROM.c`)

```c
void EEPROM_INIT(void);
void EEPROM_WriteByte(u16 address, u8 data);
u8   EEPROM_ReadByte(u16 address);
void EEPROM_WriteBlock(u16 address, const u8* buf, u16 len);
void EEPROM_ReadBlock(u16 address, u8* buf, u16 len);
```

---

## Error Codes

| Range | Category | Examples |
|-------|----------|---------|
| `0x1000–0x1FFF` | Sensor | `0x1001` read fail, `0x1002` calibration fail, `0x1003` outlier |
| `0x2000–0x2FFF` | Communication | `0x2001` SMS fail, `0x2002` no network |
| `0x3000–0x3FFF` | Memory | `0x3001` EEPROM read fail, `0x3003` checksum error |
| `0x5000–0x5FFF` | System | `0x5001` watchdog timeout, `0x5002` overload |

---

## Testing

### Host-Side Header Smoke Test

```bash
gcc -DTARGET_HOST -fsyntax-only MCAL/EEPROM/INTERNAL_EEPROM.h Config/eeprom_map.h Config/sensor_common.h
```

### Placeholder Test Runner

```bash
make test
# Compiles Test/test_phone_list_stub.c with gcc -DTARGET_HOST
# Full Unity integration planned in a follow-up feature branch
```

---

## MCU Migration

See [docs/MCU_Migration_Report.md](docs/MCU_Migration_Report.md) for a full comparison of ATmega128, STM32F411CEU6, and ESP32-S3.

**TL;DR**: **STM32F411CEU6 (BlackPill)** is the recommended next platform — 32× SRAM, 12-bit ADC with DMA, SWD debugging. Migration deferred until this cleanup is complete; the clean MCAL layer makes porting straightforward (~2–3 weeks).

---

**Version**: 2.1.0 (post-foundation-overhaul)  
**MCU**: ATmega128 @ 16 MHz  
**Last Updated**: 2026-02-21
