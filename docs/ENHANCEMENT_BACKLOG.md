# iDMS Enhancement Backlog

**Project**: Industrial Data Monitoring System (iDMS)  
**Branch**: 001-foundation-overhaul  
**Date**: 2026-02-21  
**Status**: Post-cleanup, post-defect-fix baseline  

Cross-references: [research.md](../specs/001-foundation-overhaul/research.md) | [MCU_Migration_Report.md](MCU_Migration_Report.md) | [data-model.md](../specs/001-foundation-overhaul/data-model.md)

---

## Prioritization Key

| Symbol | Meaning |
|--------|---------|
| P0 | Critical — blocks production use |
| P1 | High — significant reliability or safety impact |
| P2 | Medium — improves robustness or maintainability |
| P3 | Low — nice-to-have improvement |

| Platform Tag | Meaning |
|-------------|---------|
| ATmega128 | Implementable on current hardware |
| New MCU | Requires or strongly benefits from MCU migration |
| Platform-independent | No hardware dependency |

---

## Category 1: Reliability

### E001 — Activate and Test Watchdog Timer Integration

**Description**: `Service/watchdog_timer.c` and `Service/watchdog_timer.h` exist but their integration into the main loop has never been verified. A watchdog reset without proper `wdt_reset()` calls in the sensor polling, SMS transmission, and EEPROM write paths would cause a continuous reset loop in production. Activate watchdog in `main.c`, add `wdt_reset()` at each major loop stage, and test that a stuck loop triggers reset within ≤1 second.

**Platform dependency**: ATmega128  
**Complexity**: Low  
**Priority**: P1  
**Research reference**: research.md §9 (sendAlarmMessage timing issues that can block the main loop)

---

### E002 — EEPROM Wear Leveling for Data Log Region

**Description**: The data log region (0x280–0x680, 1024 bytes) is written sequentially from a fixed base address on every sensor poll cycle. ATmega128 EEPROM endurance is 100,000 write cycles; at a 1-second poll rate the first byte reaches end-of-life in ~28 hours. Implement a circular buffer with a 2-byte write pointer stored at the top of the Config region (0x200). On startup, scan the ring to find the write pointer; advance it modulo `EEPROM_DATA_LOG_SIZE` on each write.

**Platform dependency**: ATmega128  
**Complexity**: Medium  
**Priority**: P1  
**Research reference**: research.md §2 (EEPROM memory map), data-model.md §4 (DataLogEntry structure)

---

### E003 — SRAM Usage Guard at Boot

**Description**: Current build: 2722 B data+BSS out of 4096 B available (66%). A future feature adding a string buffer or larger sensor array could silently overflow SRAM, causing corrupt stack frames. Add a compile-time `_Static_assert` on estimated SRAM usage in `Config/sensor_common.h` and a runtime check in `main()` that computes free stack space (SP − end-of-BSS) and logs a warning if free space falls below 256 bytes.

**Platform dependency**: ATmega128  
**Complexity**: Low  
**Priority**: P1  
**Research reference**: research.md §6 (ATmega128 memory profile)

---

## Category 2: Connectivity

### E004 — GSM/SMS Retry with Exponential Back-off

**Description**: `Service/SIM_prog.c` sends AT commands with fixed delays and no retry logic. A missed SMS that goes undetected violates the alarm notification requirement. Implement a 3-attempt retry loop in `SIM_SendSMS()` with back-off delays of 1 s, 2 s, and 4 s. Check `SEND SMS OK` vs `ERROR` response parsing. Log failure to the EEPROM error log on final failure.

**Platform dependency**: ATmega128  
**Complexity**: Medium  
**Priority**: P1  
**Research reference**: research.md §9 (UART0 assignment to SIM800L)

---

### E005 — MQTT / Cloud Telemetry via ESP32-S3

**Description**: IoT operators require remote dashboards. The ESP32-S3 migration path (MCU_Migration_Report.md §5, secondary recommendation) includes native Wi-Fi and a mature MQTT stack (ESP-IDF `esp_mqtt`). Migrate iDMS to ESP32-S3 and publish `DataLogEntry` JSON payloads to an MQTT broker at each poll cycle. Retain SMS fallback for alarm notifications on broker disconnection.

**Platform dependency**: New MCU (ESP32-S3)  
**Complexity**: High  
**Priority**: P2  
**Research reference**: MCU_Migration_Report.md §Recommendation (ESP32-S3 secondary path), research.md §6

---

### E006 — AT Command Abstraction Layer

**Description**: Raw AT command strings are scattered across `Service/SIM_prog.c` and `Service/enhanced_sms.c`. This makes it impossible to swap the SIM800L for a different GSM module (e.g., SIM7600 for 4G) without rewriting the service layer. Define a `gsm_driver_t` function-pointer table (init, sendSMS, checkSignal, getNetworkStatus) and implement SIM800L as the first concrete driver. A SIM7600 driver can be added without touching the service layer.

**Platform dependency**: ATmega128  
**Complexity**: Medium  
**Priority**: P2  
**Research reference**: data-model.md §7 (GSM interface section)

---

## Category 3: User Interface

### E007 — Menu-Driven LCD Configuration

**Description**: Alarm thresholds, phone list entries, and sensor calibration offsets are compile-time constants or EEPROM values written only via firmware flash. End users in the field cannot adjust setpoints. Implement a 4-button (UP/DOWN/SELECT/BACK) navigation layer on the TopWay LCD that lets operators view live readings, scroll phone list, and adjust alarm thresholds — all stored to the Config EEPROM region (0x200–0x27F) via `EEPROM_WriteBlock()`.

**Platform dependency**: ATmega128  
**Complexity**: High  
**Priority**: P2  
**Research reference**: data-model.md §5 (Config region), data-model.md §8 (J4 LCD connector)

---

### E008 — Sensor Trend Indicator on LCD

**Description**: The LCD currently shows only the latest instantaneous reading. The `SensorReading` struct already carries `prevFiltered`, `variance`, `mean`, and `minValue`/`maxValue` fields (data-model.md §3). Display a one-character trend arrow (↑/↓/=) next to each reading based on `filteredValue - prevFiltered`, and a min/max range on the secondary LCD line. Zero additional SRAM cost because the struct fields are already computed.

**Platform dependency**: ATmega128  
**Complexity**: Low  
**Priority**: P3  
**Research reference**: data-model.md §3 (SensorReading fields), research.md §5 (Service layer MCAL access)

---

## Category 4: Maintainability

### E009 — MISRA-C:2012 Compliance Pass

**Description**: The codebase uses several MISRA-prohibited patterns: implicit integer promotion in `PhoneList.c`, variadic-macro use in `debug_log.h`, and non-constant-expression array subscripts in `app.c`. Run `cppcheck --addon=misra.py` (free) on all Service/ and MCAL/ files and resolve all mandatory and required rule violations. This is a prerequisite for any IEC 61508 or industrial certification path.

**Platform dependency**: Platform-independent  
**Complexity**: Medium  
**Priority**: P2  
**Research reference**: research.md §4 (testing strategy, static analysis mention)

---

### E010 — STM32F411 HAL Port (Primary Migration Path)

**Description**: MCU_Migration_Report.md recommends STM32F411CEU6 as the primary migration target: Cortex-M4 at 100 MHz, 512 KB Flash, 128 KB SRAM, lower SRAM pressure, no AVR toolchain dependency. Create an STM32 HAL board support package (BSP) directory at `MCAL/STM32/` implementing the same `MCAL_DelayMs`, `UART_*`, `EEPROM_*` (mapped to Flash emulation), `ADC_*`, and `DIO_*` interfaces. Guard with `#ifdef TARGET_STM32`. The service layer and app logic require zero changes.

**Platform dependency**: New MCU (STM32F411CEU6)  
**Complexity**: High  
**Priority**: P2  
**Research reference**: MCU_Migration_Report.md §Feature Mapping, research.md §6

---

## Category 5: Testing

### E011 — Unity Unit Test Suite for Service Layer

**Description**: The `make test` target currently runs a no-op host compile (T041 placeholder). Integrate the Unity test framework (research.md §4: ThrowTheSwitch/Unity, MIT license, zero dependencies) and create test files under `Test/` covering: (a) `EEPROM_WriteBlock`/`EEPROM_ReadBlock` round-trip with mock, (b) PhoneList CRUD boundary conditions (full list, duplicate number, delete from empty), (c) `SensorReading` statistics (mean, variance, min/max after N samples), (d) error handler EEPROM logging at address boundaries.

**Platform dependency**: Platform-independent (host `gcc -DTARGET_HOST`)  
**Complexity**: Medium  
**Priority**: P1  
**Research reference**: research.md §4 (Unity decision), data-model.md §3–§4

---

### E012 — Hardware-in-the-Loop (HiL) CI Script

**Description**: Extend `testing/` with a shell script that programs the ATmega128 via USBasp (`avrdude -c usbasp`), sends known UART stimuli via a USB-to-serial adapter, captures UART1 (LCD proxy) output, and asserts expected LCD strings. Run this script as a GitHub Actions job against a self-hosted runner with the board attached. Catches sensor range errors, UART frame errors, and watchdog resets that unit tests cannot detect.

**Platform dependency**: ATmega128  
**Complexity**: High  
**Priority**: P3  
**Research reference**: data-model.md §8 (J5 GSM/UART0, J4 LCD/UART1 connectors), Hardware/Wiring_Reference.md (ISP procedure)

---

## Summary Table

| ID | Title | Category | Platform | Complexity | Priority |
|----|-------|----------|----------|------------|----------|
| E001 | Watchdog Timer Integration | Reliability | ATmega128 | Low | P1 |
| E002 | EEPROM Wear Leveling | Reliability | ATmega128 | Medium | P1 |
| E003 | SRAM Usage Guard | Reliability | ATmega128 | Low | P1 |
| E004 | GSM SMS Retry + Back-off | Connectivity | ATmega128 | Medium | P1 |
| E005 | MQTT / Cloud via ESP32-S3 | Connectivity | New MCU | High | P2 |
| E006 | AT Command Abstraction Layer | Connectivity | ATmega128 | Medium | P2 |
| E007 | Menu-Driven LCD Config | UI | ATmega128 | High | P2 |
| E008 | Sensor Trend Indicator | UI | ATmega128 | Low | P3 |
| E009 | MISRA-C:2012 Compliance | Maintainability | Platform-independent | Medium | P2 |
| E010 | STM32F411 HAL Port | Maintainability | New MCU | High | P2 |
| E011 | Unity Unit Test Suite | Testing | Platform-independent | Medium | P1 |
| E012 | Hardware-in-the-Loop CI | Testing | ATmega128 | High | P3 |

---

## Suggested Next Feature Branches

Based on priority order:

1. **`002-watchdog-and-sram-guards`** (E001 + E003): Low-complexity P1 items, same code area (`main.c`, build config). Ship quickly.
2. **`003-eeprom-wear-leveling`** (E002): Self-contained EEPROM change, good Unity test candidate.
3. **`004-gsm-retry-and-abstraction`** (E004 + E006): Medium complexity, resolves a production reliability gap.
4. **`005-unity-test-suite`** (E011): Enables regression confidence before MCU migration.
5. **`006-stm32-bsp`** (E010): MCU migration, depends on E011 for regression coverage.
6. **`007-lcd-menu-and-trends`** (E007 + E008): UX improvements, lower urgency.
7. **`008-misra-compliance`** (E009): Quality gate before any certification path.
8. **`009-hil-ci`** (E012): Long-term CI investment, highest complexity.

---

*Generated from foundation-overhaul cleanup findings, research.md, and MCU_Migration_Report.md. Run `/speckit.specify` per enhancement ID to generate a full spec before starting a branch.*
