# Research: iDMS Foundation Overhaul

**Feature**: `001-foundation-overhaul` | **Date**: 2026-02-14

## Table of Contents

1. [EEPROM API Unification Strategy](#1-eeprom-api-unification-strategy)
2. [EEPROM Memory Map Redesign](#2-eeprom-memory-map-redesign)
3. [Build System Unification](#3-build-system-unification)
4. [Host-Side Testing Framework](#4-host-side-testing-framework)
5. [AVR Header Removal from Service Layer](#5-avr-header-removal-from-service-layer)
6. [MCU Migration Comparison](#6-mcu-migration-comparison)
7. [Delay Mechanism Replacement](#7-delay-mechanism-replacement)
8. [File Cleanup Impact Analysis](#8-file-cleanup-impact-analysis)
9. [Hardware/Software Pin Configuration Cross-Reference (US7)](#9-hardwaresoftware-pin-configuration-cross-reference-us7)
10. [Power Rail Verification Procedure (US7)](#10-power-rail-verification-procedure-us7)

---

## 1. EEPROM API Unification Strategy

**Unknown**: Three EEPROM APIs coexist — which should be the canonical one?

### Decision: Keep `EEPROM_WriteByte`/`EEPROM_ReadByte` as the canonical API

### Rationale

- These are the names defined in the actual MCAL implementation file
  (`INTERNAL_EEPROM.c`).
- The `EEPROM_voidWriteData`/`EEPROM_u8ReadData` names used by Service
  callers appear to be a naming convention mismatch — the callers were
  written expecting Hungarian-style names that were never added to the
  MCAL header.
- The simplest fix is to add `#define` aliases in `INTERNAL_EEPROM.h`:
  ```c
  #define EEPROM_voidWriteData(addr, data)  EEPROM_WriteByte((addr), (data))
  #define EEPROM_u8ReadData(addr)           EEPROM_ReadByte((addr))
  ```
  This avoids touching every caller (12+ call sites in error_handler.c,
  14+ in data_logger.c) while maintaining backward compatibility.
- `PhoneList.c` must be migrated from `avr/eeprom.h` to use the project
  MCAL API. This is a larger change (~12 call sites) but is required by
  Constitution Principle I (Layered Hardware Abstraction) and IV
  (Portable & Migration-Ready).

### Alternatives Considered

1. **Rename MCAL functions to match callers** — Rejected because the
   MCAL layer's naming (`EEPROM_WriteByte`) is cleaner and more
   consistent with Principle VI naming conventions.
2. **Leave PhoneList.c using avr/eeprom.h** — Rejected because it
   violates Principles I and IV and blocks MCU migration.
3. **Create a full abstraction layer with function pointers** — Rejected
   as over-engineering for a single-MCU project. Can be revisited during
   actual migration.

---

## 2. EEPROM Memory Map Redesign

**Unknown**: How should the overlapping regions be reorganized?

### Decision: Sequential allocation with explicit size guards

### Current Layout (broken)

| Region | Start | Allocated | Actual Usage | Overlap? |
|--------|-------|-----------|-------------|----------|
| Phone List | 0x000 | 512 B | ~200 B (16 contacts × 12 B + header) | No |
| System Config | 0x200 | 256 B | ~48 B (SystemConfig struct) | No |
| Data Logger | 0x300 | 256 B | **~856 B** (50 × 17 B) | **YES → overflows into 0x658** |
| Error Log | 0x400 | 256 B | ~240 B (20 × 12 B) | **YES → stomped by data logger** |

### Proposed Layout (fixed)

| Region | Start | Max Size | Actual Need | End |
|--------|-------|----------|-------------|-----|
| Phone List | 0x000 | 0x200 (512 B) | ~200 B | 0x1FF |
| System Config | 0x200 | 0x080 (128 B) | ~48 B | 0x27F |
| Data Logger | 0x280 | 0x400 (1024 B) | ~856 B → reduce to 40 entries (680 B) | 0x67F |
| Error Log | 0x680 | 0x180 (384 B) | ~240 B | 0x7FF |
| *Reserved* | 0x800 | 0x800 (2048 B) | Future use | 0xFFF |

**Total used**: 2048 B of 4096 B (50%)

### Rationale

- Data logger reduced from 50 to 40 entries to fit cleanly. 40 entries
  still covers 6+ hours at one reading per 10 minutes.
- Each region has explicit `_BASE`, `_SIZE`, and `_END` macros in
  `eeprom_map.h` with `_Static_assert` guards.
- 50% of EEPROM is reserved for future features (WiFi credentials,
  additional sensor calibration data, etc.).

### Alternatives Considered

1. **Keep 50 entries, move data logger to 0x300, error log to 0x700** —
   Workable but leaves only 256 B reserved. Reduced headroom.
2. **Use wear-leveled circular buffer across full EEPROM** — Too complex
   for this cleanup phase; can be a future enhancement.

---

## 3. Build System Unification

**Unknown**: What should the final Makefile structure look like?

### Decision: Enhanced root Makefile with auto-discovery and three modes

### Rationale

The root `Makefile` already has the right structure but has two problems:
1. `SRC_DIRS` doesn't include `./src` (where the canonical `app.c` lives)
   and includes `./Application` (which will be deleted).
2. The `test` target hard-codes specific file paths and uses `simavr` —
   it's not a host-side unit test.

### Plan

- **`make all`**: Cross-compile for ATmega128 with `-Os` optimization.
  Source discovery via wildcard across updated `SRC_DIRS`.
- **`make debug`**: Same as `all` but with `-DDEBUG_ENABLE -g -O0`.
- **`make test`**: Compile selected modules with `gcc` (host compiler)
  against Unity test framework. Runs on the build machine without
  hardware. Initially a no-op test runner that succeeds.
- **`make clean`**: Remove all `build/` artifacts.
- Remove `Debug/makefile` and all Eclipse-generated build files.

### Alternatives Considered

1. **Keep Debug/makefile for Eclipse users** — Rejected per Principle II
   (single source of truth). Eclipse can regenerate project files.
2. **Switch to CMake** — Rejected as unnecessary complexity for a
   single-target project. Re-evaluate during MCU migration.

---

## 4. Host-Side Testing Framework

**Unknown**: Which test framework and how to integrate?

### Decision: Unity (ThrowTheSwitch)

### Rationale

- **Unity** is a single .c + two .h files — zero external dependencies,
  compiles with any C compiler, ideal for embedded projects.
- Used widely in the embedded community for AVR, STM32, ESP32 projects.
- Compatible with host-side `gcc`/`clang` compilation.
- Supports `TEST_ASSERT_EQUAL`, `TEST_ASSERT_FLOAT_WITHIN`, etc. — all
  the assertions needed for sensor math and EEPROM boundary testing.
- Can be vendored directly into the repo (MIT license) under
  `Utilities/unity/`.

### Integration

- `make test` target compiles test files with host `gcc`, linking
  against the Unity runner.
- Hardware-dependent code is excluded via `#ifdef TARGET_AVR` guards in
  MCAL headers; test builds define `TARGET_HOST` instead.
- Initial test suite: empty runner that returns 0. Tests will be added
  per-module in future features.

### Alternatives Considered

1. **CUnit** — Heavier dependency, more complex setup, less common in
   embedded circles.
2. **CMocka** — Good mocking support but adds complexity not needed yet.
3. **No framework (bare printf)** — Current approach; not scalable, no
   structured assertions or reporting.

---

## 5. AVR Header Removal from Service Layer

**Unknown**: How to replace direct `avr/` includes in Service files?

### Decision: Route all hardware access through MCAL abstractions

### Files Affected

| File | Include | Replacement |
|------|---------|-------------|
| `Service/PhoneList.c` | `<avr/eeprom.h>` | Use `MCAL/EEPROM/INTERNAL_EEPROM.h` API |
| `Service/TopWayLCD_Program.c` | `"avr/delay.h"` | Use MCAL delay wrapper or `_delay_ms` via a `MCAL/Delay/delay.h` shim |
| `Service/SIM_prog.c` | `<avr/delay.h>` | Same as above |

### Delay Strategy

Create a minimal `MCAL/Delay/delay.h` shim:
```c
#ifdef TARGET_AVR
  #include <util/delay.h>
  #define MCAL_DelayMs(ms) _delay_ms(ms)
#elif defined(TARGET_HOST)
  #define MCAL_DelayMs(ms) /* no-op in test builds */
#else
  #error "Define TARGET_AVR or TARGET_HOST"
#endif
```

This keeps `avr/` includes confined to MCAL and provides a clean
abstraction for Service code to call.

### Alternatives Considered

1. **Timer-based delay** — Better for production use but more complex to
   implement. Can be a future enhancement.
2. **Leave avr/delay.h in Service** — Rejected per Principles I and IV.

---

## 6. MCU Migration Comparison

**Unknown**: Which MCU should iDMS target if migrating from ATmega128?

### Decision: Recommend **STM32F411CEU6** as primary migration target; **ESP32-S3** as secondary option if WiFi/cloud integration becomes a requirement.

### Comparison Matrix

| Criteria | ATmega128 (current) | STM32F411CEU6 | ESP32-S3-WROOM-1 |
|----------|-------------------|---------------|-------------------|
| **CPU** | 8-bit AVR @ 16 MHz | 32-bit ARM Cortex-M4F @ 100 MHz | Dual-core Xtensa LX7 @ 240 MHz |
| **FPU** | No | Yes (single-precision) | Yes (single-precision) |
| **Flash** | 128 KB | 512 KB | 16 MB (external QSPI) |
| **SRAM** | 4 KB | 128 KB | 512 KB + 8 MB PSRAM (external) |
| **EEPROM** | 4 KB native | None (emulated in Flash) | None (NVS in Flash, ~16 KB typical) |
| **ADC** | 10-bit, 8 ch | 12-bit, 16 ch, 2.4 MSPS | 12-bit, 20 ch (noisy, ~9 ENOB) |
| **UART** | 2 | 3 | 3 |
| **SPI** | 1 | 5 | 4 |
| **I2C** | 1 (TWI) | 3 | 2 |
| **WiFi** | No | No | 802.11 b/g/n |
| **Bluetooth** | No | No | BLE 5.0 |
| **USB** | No | USB OTG FS | USB OTG |
| **Watchdog** | Yes (HW) | Yes (2x IWDG + WWDG) | Yes (MWDT + RWDT) |
| **DMA** | No | Yes (16-stream) | Yes |
| **Power (active)** | ~15 mA | ~30 mA @ 100 MHz | ~80 mA (WiFi off), ~240 mA (WiFi on) |
| **Dev board cost** | ~$5 (Arduino Mega) | ~$4-6 (BlackPill) | ~$6-10 (DevKitC) |
| **Chip cost (qty 1)** | ~$6 | ~$4 | ~$3 |
| **Toolchain** | avr-gcc | arm-none-eabi-gcc + STM32CubeMX/HAL | ESP-IDF (gcc-based) |
| **RTOS support** | FreeRTOS (tight) | FreeRTOS, Zephyr, bare-metal | FreeRTOS (built-in) |
| **Community** | Large (Arduino) | Very large (STM32 ecosystem) | Very large (maker + IoT) |
| **Industrial suitability** | Good (proven) | Excellent (industrial temp range, CAN) | Good (WiFi advantage, noisier ADC) |

### iDMS Feature Mapping

| iDMS Feature | ATmega128 | STM32F411 | ESP32-S3 |
|-------------|-----------|-----------|----------|
| PT100 temperature (ADC) | PA0 (10-bit) | Any ADC pin (12-bit, lower noise, DMA) | Any ADC pin (12-bit, noisier) |
| CT current (ADC) | PA1 (10-bit) | Any ADC pin (12-bit) | Any ADC pin |
| LCD (UART) | UART0 @ 9600 | Any USART @ 9600 | Any UART @ 9600 |
| GSM SIM800L (UART) | UART1 @ 9600 | Any USART @ 9600 | Any UART @ 9600 |
| EEPROM storage | Native 4 KB | Flash emulation or external I2C EEPROM | NVS Flash partition |
| GPIO (LED, buzzer, relay) | PORTB, PORTC | Any GPIO | Any GPIO |
| Watchdog | HW WDT | IWDG (independent) | MWDT |
| ISP programming | SPI-based | SWD (faster, debug capable) | USB/UART bootloader |
| Debug | JTAG (limited) | SWD + ITM trace (excellent) | JTAG + OpenOCD |

### Migration Effort Estimate

| Task | STM32F411 Effort | ESP32-S3 Effort |
|------|-----------------|-----------------|
| MCAL/ADC rewrite | Small (HAL simplifies) | Small (ESP-IDF ADC driver) |
| MCAL/UART rewrite | Small | Small |
| MCAL/EEPROM → Flash emulation | Medium (need wear-leveling) | Small (NVS is built-in) |
| MCAL/DIO rewrite | Small | Small |
| MCAL/Delay rewrite | Small | Small |
| Makefile → CMake/ESP-IDF build | Medium | Medium (ESP-IDF is CMake) |
| Service layer changes | None (if abstractions are clean) | None |
| Application layer changes | None | None |
| **Total estimated effort** | **2-3 weeks** | **2-3 weeks** |

### Recommendation

**Primary: STM32F411CEU6 (BlackPill)**

- Best analog precision (12-bit ADC with DMA, low noise) — critical for
  industrial measurement accuracy.
- Excellent real-time performance with deterministic interrupt handling.
- 128 KB SRAM eliminates the RAM pressure problem entirely (32× current).
- SWD debugging is vastly superior to JTAG on ATmega128.
- Industrial temperature range variants available (-40°C to +85°C/+105°C).
- No WiFi means lower power consumption and no RF certification needed.
- If WiFi is needed later, add an ESP32 as a communication co-processor
  via SPI/UART.

**Secondary: ESP32-S3 (if WiFi/cloud becomes mandatory)**

- Built-in WiFi/BLE eliminates the SIM800L GSM module for sites with WiFi.
- NVS (Non-Volatile Storage) provides a built-in EEPROM replacement.
- Noisier ADC is a concern for precision measurement — may require
  external ADC (ADS1115 over I2C) for industrial accuracy.
- Higher power consumption limits battery-operated deployment.

**Decision**: Defer migration until after this cleanup is complete. The
cleanup (Principles I, IV) will make the codebase migration-ready, so
the actual port can happen in a follow-up feature branch.

---

## 7. Delay Mechanism Replacement

**Unknown**: How to replace the `for(u32 i = 0; i < 1000000; i++);` delay in main.c?

### Decision: Use `_delay_ms()` from `<util/delay.h>` wrapped in MCAL shim

### Rationale

- `_delay_ms()` is compiler-aware — it uses cycle counting and will not
  be optimized away.
- Requires `F_CPU` to be defined (already set to `16000000UL` in the
  Makefile).
- The bare `for` loop is undefined behavior when the compiler determines
  the loop body has no side effects — `-Os` optimization may remove it.
- A MCAL shim (`MCAL_DelayMs`) maintains the portability abstraction.

### Alternatives Considered

1. **`volatile` loop counter** — Works but wastes CPU cycles. Exact
   timing depends on compiler optimization level.
2. **Timer-based non-blocking delay** — Ideal for production but requires
   Timer peripheral setup. Planned as a future enhancement.

---

## 8. File Cleanup Impact Analysis

**Unknown**: Will removing files break the build or lose needed code?

### Analysis

| File to Remove | Used by Build? | Contains Unique Code? | Safe to Remove? |
|---------------|----------------|----------------------|-----------------|
| `Application/main.c` | Yes (SRC_DIRS includes `./Application`) | No — subset of root `main.c` | Yes — update SRC_DIRS |
| `Application/inc/app.h` | Yes (INCLUDE_DIRS includes `./Application/inc`) | No — subset of `inc/app.h` | Yes — update INCLUDE_DIRS |
| `Application/src/app.c` | Yes (SRC_DIRS includes `./Application/src`) | No — subset of `src/app.c` (missing enhanced features) | Yes — update SRC_DIRS |
| `Utilities/LinkedList.c` | No (not in SRC_DIRS) | Generic int linked list — unused by any module | Yes |
| `Utilities/LinkedList.h` | No (not included by any .c file) | Same | Yes |
| `IDMS/README.md` | No | Contains only `# IDMS` | Yes |
| `Debug/README.md` | No | Contains only `# IDMS` | Yes |
| `organize-files.sh` | No | One-time migration script | Yes |
| `update_includes.sh` | No | One-time migration script | Yes |
| `Debug/subdir.mk` | Only by `Debug/makefile` (stale) | Eclipse-generated, references flat layout | Yes (removing with Debug/makefile) |
| `Debug/sources.mk` | Only by `Debug/makefile` | Eclipse-generated | Yes |
| `Debug/objects.mk` | Only by `Debug/makefile` | Eclipse-generated | Yes |
| `Debug/makefile` | Manual invocation only | Eclipse-generated, builds `CodeCrafters.elf` | Yes |
| `Debug/CodeCrafters.lss` | No | Build artifact | Yes |
| `Debug/*.d` | No | Build dependency files | Yes |
| `Config/.cproject` | Eclipse only | Eclipse project config | Yes |
| `Config/.project` | Eclipse only | Eclipse project config | Yes |

**Relocate (not remove)**:
| File | From | To | Reason |
|------|------|-----|--------|
| `Debug/debug_log.c` | `Debug/` | `Service/` | Source code mixed with build artifacts |
| `Debug/debug_log.h` | `Debug/` | `Service/` | Same |

### Build Impact

After cleanup, `SRC_DIRS` must be updated to:
```makefile
SRC_DIRS = ./src ./Service ./MCAL/ADC ./MCAL/DIO ./MCAL/UART ./MCAL/EEPROM ./Config ./Test
```

And root `main.c` must be explicitly added (it's not in any SRC_DIR):
```makefile
SRCS = main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
```

`INCLUDE_DIRS` must be updated to include `./inc` and remove
`./Application`, `./Application/inc`, `./Utilities/Debug`.

### Pre/Post Verification

1. Before any deletion: `make all 2>&1 | tail -5` — capture baseline.
2. After each deletion batch: re-run build verification.
3. Final: `make all && make debug && make test` must all succeed.

---

## 9. Hardware/Software Pin Configuration Cross-Reference (US7)

**Unknown**: Do the MCAL pin configurations match the hardware design? What mismatches exist?

### Decision: Fix all mismatches in DIO_config.h and document UART peripheral assignment

### Findings — CRITICAL Issues

#### UART0/UART1 Peripheral Assignment

The hardware docs assign UART0 (PD0/PD1) to the LCD and UART1 (PE0/PE1) to the GSM module.
However, the code tells a different story:

| Peripheral | HW Doc Pin Assignment | Code Actually Uses |
|------------|----------------------|-------------------|
| **LCD (TopWay)** | UART0 — PD0(RX)/PD1(TX) | UART1 registers (0x9C, 0x9B, 0x9A, 0x9D, 0x99) → PE0/PE1 |
| **GSM (SIM800L)** | UART1 — PE0(RX)/PE1(TX) | UART0 registers (0x2C, 0x2B, 0x2A, 0x95, 0x29) → PD0/PD1 |

**Root cause**: The code in `UART_Prog.c` defines two sets of registers: `UCSRA0_REG`/`UDR0_REG` (UART0 @ 0x2B/0x2C) used by `UART_SendStringSync()` for SIM/GSM, and `UCSRA`/`UDR`/`UCSRB`/`UCSRC`/`UBBRL` (UART1 @ 0x9B/0x9C/0x9A/0x9D/0x99) used by `UART_voidInit()`/`UART_voidWriteData()` for LCD.

**Resolution**: The code's register assignments represent what actually works on the hardware. The hardware documentation has the UART assignment labels swapped. The wiring reference card will document the **actual** assignment:
- **UART0 (PD0/PD1)** → GSM (SIM800L)
- **UART1 (PE0/PE1)** → LCD (TopWay)

The Hardware_Integration.md labels will be corrected as part of US7.

#### Mixed-UART Bug in `sendAlarmMessage`

The `sendAlarmMessage()` function in `src/app.c` sends part of an SMS on UART0 (`UART_SendStringSync`) and part via UART1 (`UART_voidWriteData`). This is a pre-existing bug that must be documented and flagged for US2 defect resolution.

### Findings — Port A: All 8 ADC Pins Misconfigured

| Pin | HW Function | DIO_config.h | Required | Severity |
|-----|------------|-------------|----------|----------|
| PA0 | PT100 temp ADC input | `DIO_OUTPUT` | `DIO_INPUT` | **CRITICAL** |
| PA1 | CT current ADC input | `DIO_OUTPUT` | `DIO_INPUT` | **CRITICAL** |
| PA2–PA7 | Reserved/future ADC | `DIO_OUTPUT` | `DIO_INPUT` | Minor |

**Note**: On ATmega128, enabling ADC for a channel overrides the DDR setting via the analog multiplexer, so ADC readings may still work despite the wrong DDR. However, driving an output on an analog input pin risks damaging the signal conditioning circuit (INA118P, MCP6002) and corrupting readings through contention.

### Findings — Port B: ISP Pin Mismatch

| Pin | HW Function | DIO_config.h | Required | Severity |
|-----|------------|-------------|----------|----------|
| PB0 | External timer input (T0) | `DIO_OUTPUT` | `DIO_INPUT` | Minor |
| PB3 | ISP MISO (input from target) | `DIO_OUTPUT` | `DIO_INPUT` | Moderate |

### Findings — Port C: Control Output Pins Misconfigured

| Pin | HW Function | DIO_config.h | Required | Severity |
|-----|------------|-------------|----------|----------|
| PC0 | GSM RESET control (push low to reset) | `DIO_INPUT` | `DIO_OUTPUT` | **CRITICAL** |
| PC2 | Relay 1 — alarm output | `DIO_INPUT` | `DIO_OUTPUT` | **CRITICAL** |
| PC3 | Relay 2 — fault output | `DIO_INPUT` | `DIO_OUTPUT` | **CRITICAL** |

### Findings — Port D: TX Pin Direction

| Pin | HW Function | DIO_config.h | Required | Severity |
|-----|------------|-------------|----------|----------|
| PD1 | UART0 TX (GSM per actual code) | `DIO_INPUT` | `DIO_OUTPUT` | Moderate |

### Findings — Ports E, F, G: Entirely Missing

DIO_config.h only defines Ports A–D. Ports E, F, and G have no configuration entries despite being used by the hardware:
- **Port E**: PE0 (UART1 RX/LCD), PE1 (UART1 TX/LCD), PE3 (future SD CS), PE4–PE5 (SPI CS)
- **Port F**: PF0–PF3 (alternate ADC), PF4–PF7 (JTAG)
- **Port G**: PG0–PG1 (crystal), PG2 (watchdog reset), PG3 (user button), PG4 (status LED)

### Findings — `MDIO_vInt()` Initializer Commented Out

The DIO initialization function `MDIO_vInt()` in `DIO_program.c` is **commented out**, meaning even the existing (incorrect) config macros are never applied at runtime. The ATmega128 resets all DDR registers to 0x00 (all inputs) by default, so:
- Port A analog inputs work by accident (reset default = input)
- Port C control outputs (PC0, PC2, PC3) never drive outputs (reset default = input)
- Port B LEDs/buzzer never drive outputs unless set elsewhere

### Summary of Required Fixes

| Priority | Fix | Files |
|----------|-----|-------|
| CRITICAL | Fix Port A PA0–PA7 direction to DIO_INPUT | MCAL/DIO/DIO_config.h |
| CRITICAL | Fix Port C PC0, PC2, PC3 direction to DIO_OUTPUT | MCAL/DIO/DIO_config.h |
| CRITICAL | Add Port E configuration (PE0 input, PE1 output, etc.) | MCAL/DIO/DIO_config.h |
| MODERATE | Fix PD1 and PB0/PB3 directions | MCAL/DIO/DIO_config.h |
| MODERATE | Add Port F and Port G configurations | MCAL/DIO/DIO_config.h |
| MODERATE | Correct UART labels in Hardware_Integration.md and PCB_Schematic.md | Hardware/*.md |
| INFO | Flag `sendAlarmMessage()` mixed-UART bug for US2 | src/app.c |
| INFO | Uncomment/fix MDIO_vInt() or document that DIO init must be called | MCAL/DIO/DIO_program.c |

### Alternatives Considered

1. **Only document mismatches, don't fix** — Rejected per user clarification:
   fix all SW/HW pin config mismatches as part of US7.
2. **Swap UART wiring at hardware level** — Rejected; the code's register
   assignments represent what actually works. Easier to fix the documentation.
3. **Defer Port F/G to future** — Acceptable for missing ports not actively
   used, but config entries should still exist with correct defaults for completeness.

---

## 10. Power Rail Verification Procedure (US7)

**Unknown**: What are the expected voltage ranges for the go/no-go checks?

### Decision: Derive from component datasheets in BOM and Hardware_Integration.md

### Power Rails and Tolerances

| Rail | Nominal | Min | Max | Regulator | Load | Measurement Point |
|------|---------|-----|-----|-----------|------|-------------------|
| +12V input | 12.0V | 10.0V | 15.0V | External PSU | All downstream | J1 terminals |
| +5V digital | 5.0V | 4.75V | 5.25V | LM7805 (±5%) | MCU, ISP, LEDs | U2 output / C4+ |
| +3.3V logic | 3.3V | 3.2V | 3.4V | AMS1117-3.3 (±3%) | Digital logic | U3 output / C5+ |
| +5V analog | 5.0V | 4.90V | 5.10V | LM2596 (±2%) | Sensor conditioning | U4 output / C12+ |
| +3.7V GSM | 3.7V | 3.6V | 3.8V | LM2596-ADJ (±3%) | SIM800L | U7 output / C13+ |

### Go/No-Go Sequence

1. **Before power-on**: Visual inspection, check for shorts with multimeter continuity mode
2. **Apply 12V**: Verify J1 input voltage 10–15V
3. **Measure +5V digital**: 4.75–5.25V → GO. Outside range → STOP (check U2, input voltage)
4. **Measure +3.3V logic**: 3.2–3.4V → GO. Outside range → STOP (check U3)
5. **Measure +5V analog**: 4.90–5.10V → GO. Outside range → STOP (check U4, inductor L2)
6. **Measure +3.7V GSM**: 3.6–3.8V → GO. Outside range → STOP (check U7, adjust feedback resistor)
7. **Check current draw**: <200 mA at idle (no modules connected) → GO

Only after all rails pass: connect ISP programmer, sensors, LCD, GSM module.
