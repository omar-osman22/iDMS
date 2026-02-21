# Data Model: iDMS Foundation Overhaul

**Feature**: `001-foundation-overhaul` | **Date**: 2026-02-14

## Overview

This document defines the key data structures, memory layouts, and entity
relationships for the iDMS Foundation Overhaul. The primary focus is the
EEPROM memory map (the only persistent data store) and the core runtime
data structures that flow through the system.

---

## 1. EEPROM Memory Map

The EEPROM is the single persistent storage medium (4096 bytes on
ATmega128). All regions are defined in `Config/eeprom_map.h`.

### Region Allocation

```
Address   Region              Size     Owner Module        Write Freq
─────────────────────────────────────────────────────────────────────
0x000     PHONE_LIST           512 B   Service/PhoneList    Rare (user action)
0x200     SYSTEM_CONFIG        128 B   Config/system_config Rare (config change)
0x280     DATA_LOG            1024 B   Service/data_logger  Every 10 readings (~10 min)
0x680     ERROR_LOG            384 B   Service/error_handler On error occurrence
0x800     RESERVED            2048 B   (future use)         N/A
─────────────────────────────────────────────────────────────────────
Total:    4096 B (0x000–0xFFF)
Used:     2048 B (50%)
Reserved: 2048 B (50%)
```

### Region Detail

#### Phone List (0x000–0x1FF)

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0x000 | count | u8 | 1 B | Number of stored contacts |
| 0x001 | entries[] | PhoneEntry[16] | 16 × 13 B = 208 B | Phone number entries |

**PhoneEntry** (13 bytes):
| Offset | Field | Type | Size |
|--------|-------|------|------|
| +0 | type | u8 | 1 B | 0=SMS, 1=Call, 2=Both |
| +1 | number | char[12] | 12 B | Phone number string (null-terminated) |

**Wear estimate**: ~10 writes/year (user manually adds/removes contacts). 
Lifetime @ 100K cycles: effectively unlimited.

#### System Config (0x200–0x27F)

| Offset | Field | Type | Size | Default |
|--------|-------|------|------|---------|
| 0x200 | magic | u16 | 2 B | 0xCF01 |
| 0x202 | tempMin | f32 | 4 B | -10.0 |
| 0x206 | tempMax | f32 | 4 B | 80.0 |
| 0x20A | currentMax | f32 | 4 B | 30.0 |
| 0x20E | filterAlpha | f32 | 4 B | 0.3 |
| 0x212 | outlierThreshold | f32 | 4 B | 3.0 |
| 0x216 | alarmDelay | u16 | 2 B | 300 (seconds) |
| 0x218 | retryCount | u8 | 1 B | 3 |
| 0x219 | retryDelay | u16 | 2 B | 30 (seconds) |
| 0x21B | debugLevel | u8 | 1 B | 1 |
| 0x21C | displayInterval | u16 | 2 B | 5 (seconds) |
| 0x21E | powerSaveMode | u8 | 1 B | 0 (disabled) |
| 0x21F | calibrationSamples | u8 | 1 B | 20 |
| 0x220 | checksum | u8 | 1 B | CRC-8 of 0x200–0x21F |

**Wear estimate**: ~50 writes/year (occasional threshold tuning). 
Lifetime @ 100K cycles: effectively unlimited.

#### Data Log (0x280–0x67F)

Circular buffer of sensor readings with index tracking.

| Offset | Field | Type | Size |
|--------|-------|------|------|
| 0x280 | writeIndex | u8 | 1 B |
| 0x281 | entryCount | u8 | 1 B |
| 0x282 | entries[] | LogEntry[40] | 40 × 25 B = 1000 B |

**LogEntry** (25 bytes):
| Offset | Field | Type | Size |
|--------|-------|------|------|
| +0 | timestamp | u32 | 4 B | System uptime in seconds |
| +4 | temperature | f32 | 4 B | Filtered temperature reading |
| +8 | current | f32 | 4 B | Filtered current reading |
| +12 | power | f32 | 4 B | Calculated power |
| +16 | tempRaw | u16 | 2 B | Raw ADC value |
| +18 | currentRaw | u16 | 2 B | Raw ADC value |
| +20 | alarmFlags | u8 | 1 B | Bit field: temp_high, temp_low, current_high |
| +21 | reserved | u8[4] | 4 B | Future use |

**Wear estimate**: 1 write every 10 readings × 10-second intervals = 
1 write per 100 seconds per byte position. Circular across 40 entries.
Worst-case single byte: 100K cycles / (864 writes/day) = ~115 days. 
**Action required**: Implement write-skipping (only write if values 
changed beyond threshold) to extend lifetime by ~10×.

#### Error Log (0x680–0x7FF)

Circular buffer of error records.

| Offset | Field | Type | Size |
|--------|-------|------|------|
| 0x680 | writeIndex | u8 | 1 B |
| 0x681 | entryCount | u8 | 1 B |
| 0x682 | entries[] | ErrorRecord[20] | 20 × 19 B = 380 B |

**ErrorRecord** (19 bytes):
| Offset | Field | Type | Size |
|--------|-------|------|------|
| +0 | timestamp | u32 | 4 B |
| +4 | errorCode | u16 | 2 B |
| +6 | severity | u8 | 1 B | 0=INFO, 1=WARN, 2=ERROR, 3=CRITICAL |
| +7 | moduleId | u8 | 1 B | Identifies source module |
| +8 | context | u8[8] | 8 B | Module-specific context data |
| +16 | resolved | u8 | 1 B | 0=active, 1=resolved |
| +17 | reserved | u8[2] | 2 B | Future use |

**Wear estimate**: Errors are infrequent (~10/day under normal operation).
Lifetime: effectively unlimited.

---

## 2. Runtime Data Structures

### SensorReading (enhanced — `inc/app.h`)

```
SensorReading
├── rawValue      : u16    — Direct ADC sample (0–1023)
├── filteredValue : f32    — After IIR filter
├── prevFiltered  : f32    — Previous filtered value (IIR state)
├── variance      : f32    — Running variance for outlier detection
├── mean          : f32    — Running mean
├── sampleCount   : u16    — Total samples since calibration
├── isCalibrated  : u8     — 1 after calibration complete
├── minValue      : f32    — Minimum observed since reset
└── maxValue      : f32    — Maximum observed since reset
```

### AppState (`inc/app.h` — to be moved from header to `src/app.c`)

```
AppState
├── temperature     : SensorReading
├── current         : SensorReading
├── power           : f32
├── isAlarmActive   : u8
├── lastAlarmTime   : u32
├── systemTime      : u32
├── alarmCount      : u16
└── isInitialized   : u8
```

### SystemConfig (`Config/system_config.h`)

```
SystemConfig
├── tempMinThreshold    : f32
├── tempMaxThreshold    : f32
├── currentMaxThreshold : f32
├── filterAlpha         : f32
├── outlierThreshold    : f32
├── alarmDelaySeconds   : u16
├── smsRetryCount       : u8
├── smsRetryDelay       : u16
├── debugLevel          : u8
├── displayInterval     : u16
├── powerSaveMode       : u8
└── calibrationSamples  : u8
```

---

## 3. Key Entities (from spec)

### EEPROM Memory Region

Defined in `Config/eeprom_map.h`:
```
EepromRegion
├── name          : string (macro prefix, e.g., EEPROM_PHONE_LIST)
├── baseAddress   : u16   (e.g., 0x000)
├── maxSize       : u16   (e.g., 512)
├── endAddress    : u16   (computed: base + size - 1)
├── ownerModule   : string (e.g., "Service/PhoneList")
└── writeFrequency: string (e.g., "Rare — user action only")
```

Enforced via compile-time `_Static_assert` guards to prevent overlap.

### MCU Candidate (research document entity)

```
McuCandidate
├── name           : string
├── cpuArch        : string (e.g., "ARM Cortex-M4F")
├── clockSpeed     : u32 (MHz)
├── flashSize      : u32 (KB)
├── sramSize       : u32 (KB)
├── eepromEquiv    : string (e.g., "Flash emulation", "NVS")
├── adcResolution  : u8 (bits)
├── adcChannels    : u8
├── uartCount      : u8
├── hasWiFi        : bool
├── hasBLE         : bool
├── powerActive    : u16 (mA)
├── chipCost       : f32 ($)
├── toolchain      : string
├── migrationEffort: string (Small/Medium/Large)
└── recommendation : string (Primary/Secondary/Not recommended)
```

### Enhancement Idea (backlog entity)

```
EnhancementIdea
├── id            : string (e.g., "ENH-001")
├── title         : string
├── description   : string
├── category      : enum {Reliability, Connectivity, UI, Maintainability, Testing}
├── platformDep   : enum {CurrentMCU, NewMCU, PlatformIndependent}
├── complexity    : enum {Low, Medium, High}
└── suggestedPrio : u8 (1 = highest)
```

---

## 4. State Transitions

### Alarm State Machine

```
                    ┌─────────────┐
                    │   NORMAL    │
                    │ (no alarms) │
                    └──────┬──────┘
                           │ threshold exceeded
                           │ (temp or current)
                    ┌──────▼──────┐
                    │  ALARMING   │
                    │ (SMS sent)  │
                    └──────┬──────┘
                           │ alarm_delay elapsed
                           │ + still exceeded?
                  ┌────────┴────────┐
                  │ YES             │ NO
           ┌──────▼──────┐  ┌──────▼──────┐
           │  RE-ALARM   │  │  CLEARING   │
           │ (re-send)   │  │ (cooldown)  │
           └──────┬──────┘  └──────┬──────┘
                  │                │ values normal
                  │ alarm_delay    │ for 60s
                  └───►ALARMING    └───►NORMAL
```

### System Lifecycle

```
POWER_ON → INIT → CALIBRATING → RUNNING ──► ERROR_RECOVERY
              │                    │              │
              └── config corrupt ──┘              │
                  (load defaults)                 │
                                    watchdog ◄────┘
                                    timeout
                                      │
                                   RESET
```

---

## 5. Shared Constants (`Config/sensor_common.h`)

| Constant | Value | Used By |
|----------|-------|---------|
| `VREF` | 4845 (mV) | PT100_config.h, CTcurrent_config.h |
| `ADC_RESOLUTION` | 1024 | PT100, CTcurrent calculations |
| `SOURCE_VOLTAGE` | 220 (V) | Power calculation |
| `SYSTEM_TICK_MS` | 10 | Main loop timing |

---

## 6. Pin Configuration Model (`MCAL/DIO/DIO_config.h`)

The ATmega128 has 7 I/O ports (A–G). Each pin's direction is configured via
`DIO_PIN_Xn_DIR` macros. The corrected configuration matches the hardware
design documented in `Hardware/Hardware_Integration.md`.

### Port A — ADC Inputs (all INPUT for analog signal conditioning)

| Pin | Function | Direction | SW Macro |
|-----|----------|-----------|----------|
| PA0 | PT100 temperature (ADC0) | INPUT | `DIO_PIN_A0_DIR` |
| PA1 | CT current sensor (ADC1) | INPUT | `DIO_PIN_A1_DIR` |
| PA2–PA7 | Reserved/future ADC | INPUT | `DIO_PIN_A2_DIR`–`DIO_PIN_A7_DIR` |

### Port B — ISP, LEDs, Buzzer

| Pin | Function | Direction | SW Macro |
|-----|----------|-----------|----------|
| PB0 | External timer input (T0) | INPUT | `DIO_PIN_B0_DIR` |
| PB1 | ISP SCK | OUTPUT | `DIO_PIN_B1_DIR` |
| PB2 | ISP MOSI | OUTPUT | `DIO_PIN_B2_DIR` |
| PB3 | ISP MISO | INPUT | `DIO_PIN_B3_DIR` |
| PB4 | PWM output (fan) | OUTPUT | `DIO_PIN_B4_DIR` |
| PB5 | Status LED 1 | OUTPUT | `DIO_PIN_B5_DIR` |
| PB6 | Status LED 2 | OUTPUT | `DIO_PIN_B6_DIR` |
| PB7 | Alarm buzzer | OUTPUT | `DIO_PIN_B7_DIR` |

### Port C — GSM Control, Relays, Inputs

| Pin | Function | Direction | SW Macro |
|-----|----------|-----------|----------|
| PC0 | GSM RESET control | OUTPUT | `DIO_PIN_C0_DIR` |
| PC1 | GSM STATUS input | INPUT | `DIO_PIN_C1_DIR` |
| PC2 | Relay 1 — alarm | OUTPUT | `DIO_PIN_C2_DIR` |
| PC3 | Relay 2 — fault | OUTPUT | `DIO_PIN_C3_DIR` |
| PC4 | External interrupt input | INPUT | `DIO_PIN_C4_DIR` |
| PC5 | System enable/disable | INPUT | `DIO_PIN_C5_DIR` |
| PC6 | Reserved | INPUT | `DIO_PIN_C6_DIR` |
| PC7 | Emergency stop (active low) | INPUT | `DIO_PIN_C7_DIR` |

### Port D — UART0 (GSM), External Interrupts

| Pin | Function | Direction | SW Macro |
|-----|----------|-----------|----------|
| PD0 | UART0 RX (GSM — per actual code) | INPUT | `DIO_PIN_D0_DIR` |
| PD1 | UART0 TX (GSM — per actual code) | OUTPUT | `DIO_PIN_D1_DIR` |
| PD2 | INT0 — emergency stop | INPUT | `DIO_PIN_D2_DIR` |
| PD3 | INT1 — user input | INPUT | `DIO_PIN_D3_DIR` |
| PD4–PD7 | Reserved | INPUT | `DIO_PIN_D4_DIR`–`DIO_PIN_D7_DIR` |

### Port E — UART1 (LCD), SPI CS (NEW — not in current DIO_config.h)

| Pin | Function | Direction | SW Macro |
|-----|----------|-----------|----------|
| PE0 | UART1 RX (LCD — per actual code) | INPUT | `DIO_PIN_E0_DIR` |
| PE1 | UART1 TX (LCD — per actual code) | OUTPUT | `DIO_PIN_E1_DIR` |
| PE2 | XCK1 (external clock) | INPUT | `DIO_PIN_E2_DIR` |
| PE3 | SD card CS (future) | OUTPUT | `DIO_PIN_E3_DIR` |
| PE4 | SPI CS 1 | OUTPUT | `DIO_PIN_E4_DIR` |
| PE5 | SPI CS 2 | OUTPUT | `DIO_PIN_E5_DIR` |
| PE6–PE7 | Reserved | INPUT | `DIO_PIN_E6_DIR`–`DIO_PIN_E7_DIR` |

### Port F — ADC/JTAG (NEW — not in current DIO_config.h)

| Pin | Function | Direction |
|-----|----------|-----------|
| PF0–PF3 | Alternate ADC channels | INPUT |
| PF4 | JTAG TCK | INPUT |
| PF5 | JTAG TMS | INPUT |
| PF6 | JTAG TDO | OUTPUT |
| PF7 | JTAG TDI | INPUT |

### Port G — Additional I/O (NEW — not in current DIO_config.h)

| Pin | Function | Direction |
|-----|----------|-----------|
| PG0 | Crystal driver output | OUTPUT |
| PG1 | Crystal driver input | INPUT |
| PG2 | Watchdog reset output | OUTPUT |
| PG3 | User button input | INPUT |
| PG4 | Status indicator output | OUTPUT |

### UART Peripheral Assignment (CORRECTED from HW docs)

The hardware documentation labels UART0→LCD and UART1→GSM, but the actual
firmware code uses:

| UART | Registers | Actual Assignment | Pins |
|------|-----------|-------------------|------|
| UART0 | UDR0 @0x2C, UCSRA0 @0x2B | **GSM (SIM800L)** | PD0(RX)/PD1(TX) |
| UART1 | UDR @0x9C, UCSRA @0x9B | **LCD (TopWay)** | PE0(RX)/PE1(TX) |

---

## 7. Power Rail Verification Model

| Rail | Regulator | Nominal | Go Range | No-Go Action |
|------|-----------|---------|----------|--------------|
| +12V input | External PSU | 12.0V | 10.0–15.0V | Check PSU / fuse F1 |
| +5V digital | LM7805 (U2) | 5.0V | 4.75–5.25V | Check U2, input voltage |
| +3.3V logic | AMS1117 (U3) | 3.3V | 3.2–3.4V | Check U3, solder joints |
| +5V analog | LM2596 (U4) | 5.0V | 4.90–5.10V | Check U4, inductor L2 |
| +3.7V GSM | LM2596-ADJ (U7) | 3.7V | 3.6–3.8V | Check U7, feedback resistor |

---

## 8. Connector Pinout Model

| Connector | Type | Pins | Connected To |
|-----------|------|------|-------------|
| J1 | 2-pin screw terminal (5.08mm) | +12V, GND | Power supply input |
| J2 | 3-pin screw terminal (3.81mm) | RTD+, RTD-, Shield | PT100 sensor |
| J3 | 2-pin screw terminal (5.08mm) | CT S1, CT S2 | Current transformer |
| J4 | 4-pin header (2.54mm) | +5V, GND, TX(PE1), RX(PE0) | TopWay LCD (UART1) |
| J5 | 6-pin socket header (2.54mm) | +3.7V, GND, TX(PD1), RX(PD0), RST(PC0), STATUS(PC1) | SIM800L GSM |
| J6 | SMA female (50Ω) | Antenna | GSM antenna |
| J7 | Micro SIM socket | SIM contacts | SIM card |
| J8 | 2×3 pin header (2.54mm) | MISO(PB3), VCC, SCK(PB1), MOSI(PB2), RST, GND | ISP programmer |
