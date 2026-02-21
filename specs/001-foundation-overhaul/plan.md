# Implementation Plan: iDMS Foundation Overhaul

**Branch**: `001-foundation-overhaul` | **Date**: 2026-02-21 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/001-foundation-overhaul/spec.md`

## Summary

Comprehensive foundation overhaul of the iDMS (Industrial Digital Measurement System) firmware: repository cleanup (US1), defect resolution (US2), build system unification (US3), MCU migration research (US4), documentation update (US5), enhancement backlog (US6), and hardware wiring reference with pin configuration fixes (US7). The technical approach prioritizes removing duplicates, fixing EEPROM overlap/API mismatches, creating shared headers (`eeprom_map.h`, `sensor_common.h`, `delay.h`), unifying the Makefile, fixing DIO_config.h pin mismatches, and producing a consolidated wiring reference card.

## Technical Context

**Language/Version**: C99 (avr-gcc 7.3+)
**Primary Dependencies**: avr-libc, avr-gcc toolchain, avrdude, host gcc (for tests)
**Storage**: ATmega128 internal EEPROM (4 KB), Flash (128 KB)
**Testing**: Unity (ThrowTheSwitch) — vendored, host-compiled via `make test` with `TARGET_HOST` flag
**Target Platform**: ATmega128 @ 16 MHz (TQFP-64); host x86/ARM Linux for tests
**Project Type**: Single embedded firmware project (bare-metal, no OS)
**Performance Goals**: Real-time sensor sampling at 10 Hz, LCD update every 5 s, SMS alarm within documented latency
**Constraints**: Flash ≤80%, RAM ≤75% of capacity. EEPROM 4 KB total. Compiler warnings must not increase above baseline (29).
**Scale/Scope**: ~35 source files post-cleanup, single developer, 7 user stories

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

| # | Principle | Status | How Satisfied / Violation Justification |
|---|-----------|--------|----------------------------------------|
| I | Layered Hardware Abstraction | **PASS** | US2 removes `avr/eeprom.h` from Service (PhoneList.c), `avr/delay.h` from Service (TopWayLCD, SIM_prog), routing through MCAL. US7 fixes DIO_config.h pin directions. |
| II | Single Source of Truth | **PASS** | US1 deletes all duplicates (Application/, LinkedList, dual build systems). US2 creates `eeprom_map.h` (single EEPROM map), `sensor_common.h` (single VREF). |
| III | Safety & Reliability First | **PASS** | EEPROM overlap fix (US2) prevents data corruption. Compile-time `_Static_assert` guards prevent future overlap. Delay mechanism replacement prevents UB from optimized-away loops. |
| IV | Portable & Migration-Ready | **PASS** | `MCAL/Delay/delay.h` shim with `TARGET_AVR`/`TARGET_HOST` guards. All `avr/` headers confined to MCAL. US4 documents migration path. |
| V | Testable by Design | **PASS** | `make test` target created (US3). Unity framework integrated. `TARGET_HOST` compile-time switch. Boundary tests for EEPROM overlap guards (T009b, T030b). |
| VI | Consistent Naming & Style | **PASS** | New headers follow `SCREAMING_SNAKE_CASE` for macros, `MODULE_ActionNoun` for functions. `#define` aliases bridge legacy callers. |
| VII | Living Documentation | **PASS** | US5 updates README. US7 produces wiring reference card. Header comments added to all new/modified files (T009a, T030a). Pin assignments documented and cross-referenced. |

**Gate result**: PASS — No violations. Proceed to Phase 0.

**Post-Phase 1 Re-evaluation (2026-02-21)**: PASS. The UART0↔UART1 label swap discovered in research §9 does not violate any principle — the code correctly uses distinct register sets for each peripheral; only the documentation labels were swapped. US7 corrects both the DIO_config.h pin directions (Principle I compliance) and the Hardware_Integration.md UART labels (Principle VII). The `sendAlarmMessage()` mixed-UART bug is flagged for US2 defect resolution (Principle III — Safety).

## Project Structure

### Documentation (this feature)

```text
specs/001-foundation-overhaul/
├── plan.md              # This file
├── spec.md              # Feature specification (7 user stories)
├── research.md          # Phase 0 output (8 research topics + US7 HW analysis)
├── data-model.md        # Phase 1 output (EEPROM map, structs, state machines)
├── quickstart.md        # Phase 1 output (build/verify commands)
├── tasks.md             # Phase 2 output (64+ tasks across 9 phases)
├── contracts/           # Phase 1 output (4 header contracts + 1 new)
│   ├── eeprom_map.h
│   ├── eeprom_api.h
│   ├── sensor_common.h
│   ├── delay_shim.h
│   └── dio_config_corrected.h  # NEW: corrected pin config contract
└── checklists/
    └── requirements.md
```

### Source Code (repository root — post-cleanup layout)

```text
iDMS/
├── main.c                    ← Entry point
├── Makefile                  ← THE build system (sole authority)
├── README.md                 ← Project documentation
│
├── inc/app.h                 ← Application header (AppState, SensorReading)
├── src/app.c                 ← Application implementation
│
├── MCAL/                     ← Hardware abstraction (MCU-specific)
│   ├── ADC/                  ← Analog-to-digital converter
│   ├── DIO/                  ← Digital I/O (pin config corrected by US7)
│   ├── EEPROM/               ← Internal EEPROM driver (unified API)
│   ├── UART/                 ← Serial communication (UART0=LCD, UART1=GSM)
│   └── Delay/delay.h         ← Portable delay shim (NEW)
│
├── Service/                  ← Sensor drivers & system services
│   ├── debug_log.*           ← UART debug logging (relocated from Debug/)
│   ├── data_logger.*         ← EEPROM data recording
│   ├── error_handler.*       ← Centralized error management
│   └── ...                   ← PT100, CT, SIM, LCD, SMS, PhoneList, watchdog
│
├── Config/                   ← System configuration
│   ├── system_config.*       ← Runtime config (EEPROM-backed)
│   ├── eeprom_map.h          ← EEPROM address map with _Static_assert (NEW)
│   └── sensor_common.h       ← Shared constants: VREF, ADC_RESOLUTION (NEW)
│
├── Utilities/                ← Type definitions & macros (LinkedList removed)
│   ├── STD_TYPES.h
│   ├── BIT_MATH.h
│   └── errorStates.h
│
├── Hardware/                 ← Hardware documentation
│   └── Wiring_Reference.md   ← Consolidated wiring card (NEW from US7)
│
├── docs/                     ← Stakeholder-facing documents (NEW)
│   ├── MCU_Migration_Report.md
│   └── ENHANCEMENT_BACKLOG.md
│
├── Test/                     ← Test source files
└── testing/                  ← Test scripts & reports
```

**Structure Decision**: Single embedded project. No Application/ directory (deleted). Root `main.c` as entry. Four-layer architecture: Utilities → MCAL → Service → Application (inc/src). Config available to all layers.

## Complexity Tracking

No constitution violations to justify. All principles satisfied by design.
