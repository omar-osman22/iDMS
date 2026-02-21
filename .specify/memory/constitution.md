<!--
  Sync Impact Report
  ==================
  Version change: N/A → 1.0.0 (initial adoption)
  Modified principles: N/A (first ratification)
  Added sections:
    - Core Principles (7 principles)
    - Technical Constraints & Standards
    - Development Workflow & Quality Gates
    - Governance
  Removed sections: N/A
  Templates requiring updates:
    - .specify/templates/plan-template.md ✅ compatible (Constitution Check
      section already exists as a generic gate)
    - .specify/templates/spec-template.md ✅ compatible (requirements and
      acceptance criteria align with Principle III and V)
    - .specify/templates/tasks-template.md ✅ compatible (phased structure
      aligns with layered architecture principle)
    - .specify/templates/agent-file-template.md ✅ compatible (no outdated
      references)
    - .specify/templates/checklist-template.md ✅ compatible
  Follow-up TODOs: None
-->

# iDMS Constitution

## Core Principles

### I. Layered Hardware Abstraction (NON-NEGOTIABLE)

All firmware MUST follow a strict four-layer architecture with
unidirectional dependency flow:

- **MCAL (Microcontroller Abstraction Layer)**: Register-level drivers.
  MUST NOT reference any Service or Application code. Each peripheral
  module MUST expose a clean `_interface.h` header as its sole public
  API.
- **Service**: Sensor drivers, communication protocols, and system
  services (filtering, logging, SMS, watchdog). MUST depend only on
  MCAL and Utilities. MUST NOT call Application-layer functions.
- **Application**: Orchestration, state machines, threshold logic.
  Depends on Service and MCAL layers.
- **Utilities**: Type definitions, bit manipulation macros, error state
  enums. Zero-dependency; available to all layers.

**Rationale**: Enforcing layer boundaries makes MCU migration (e.g.,
ATmega128 → STM32 or ESP32) possible by replacing only the MCAL layer
without rewriting business logic. It also prevents circular
dependencies that cause untestable, tightly-coupled code.

### II. Single Source of Truth

Every concept, configuration, and implementation MUST exist in exactly
one canonical location:

- No duplicate source files (e.g., two `app.c` / `main.c` variants).
  If a newer version supersedes an older one, the older MUST be removed.
- One authoritative build system. Stale or generated build artifacts
  (e.g., IDE-generated makefiles) MUST NOT coexist with the project's
  primary build system unless clearly isolated and documented.
- Shared constants (e.g., `VREF`, EEPROM base addresses) MUST be
  defined in a single header and included where needed—never
  redefined in multiple files.
- EEPROM memory layout MUST be specified in a single `eeprom_map.h`
  header with named address macros and size guards to prevent region
  overlap.

**Rationale**: Duplication causes divergence, stale artifacts confuse
contributors, and address collisions corrupt persistent data silently.

### III. Safety & Reliability First

This is an industrial monitoring system. Reliability requirements MUST
take precedence over feature velocity:

- Every sensor reading path MUST include filtering and outlier
  rejection before being used for threshold comparison or display.
- Alarm thresholds MUST be validated at configuration time; out-of-range
  values MUST be rejected, and safe defaults MUST be loaded.
- The watchdog (hardware or software) MUST be active in all non-debug
  builds. Watchdog-fed tasks MUST have documented maximum execution
  times.
- All EEPROM writes MUST include checksum or CRC verification.
  Corrupted reads MUST fall back to compile-time defaults and log an
  error.
- Critical error conditions MUST trigger the alarm chain (SMS + local
  buzzer/LED) within a bounded, documented latency.

**Rationale**: An industrial measurement system that silently reports
wrong data or fails to alert operators is worse than one that is
offline. Fail-safe behavior is mandatory.

### IV. Portable & Migration-Ready

The codebase MUST be structured so that migrating to a different MCU
family requires changes only in MCAL and the linker/startup files:

- Application and Service code MUST NOT use `avr/*` headers or
  compiler intrinsics directly. All hardware access MUST go through
  MCAL abstractions.
- Peripheral configuration (pin assignments, baud rates, ADC channels)
  MUST be centralized in `_config.h` files, not scattered across
  implementation code.
- Build scripts MUST parameterize the target MCU, toolchain prefix, and
  linker script so that a new target can be added by providing a new
  configuration block—not by forking the entire build.

**Rationale**: The project is evaluating migration from ATmega128 to
more capable platforms (STM32, ESP32). Code that leaks hardware
specifics into business logic makes migration a rewrite instead of a
port.

### V. Testable by Design

Every module MUST be independently compilable and testable on the host
(x86/ARM Linux) with hardware dependencies stubbed or mocked:

- Public module APIs MUST be defined in `_interface.h` headers with no
  implementation details exposed.
- Hardware interactions MUST be injected via function pointers or
  compile-time switches so that test harnesses can substitute stubs.
- A unit test framework (Unity or equivalent lightweight C framework)
  MUST be integrated into the build system with a `make test` target
  that runs without target hardware.
- New modules MUST NOT be merged without at least boundary-condition
  and nominal-path tests.

**Rationale**: Embedded code without host-side tests can only be
validated on hardware, which is slow, error-prone, and blocks parallel
development. Desktop-testable modules catch logic bugs early.

### VI. Consistent Naming & Style

All new and modified code MUST follow a single naming convention:

- **Functions**: `MODULE_ActionNoun` (e.g., `UART_Init`,
  `CONFIG_LoadDefaults`, `DATALOG_LogReading`).
- **Types**: `PascalCase` for structs/enums (e.g., `SensorReading`,
  `AlarmState`). Typedefs for fixed-width types use project-standard
  aliases (`u8`, `u16`, `u32`, `s8`, `s16`, `s32`, `f32`).
- **Macros/constants**: `SCREAMING_SNAKE_CASE`
  (e.g., `ADC_CHANNEL_TEMP`, `EEPROM_PHONE_LIST_BASE`).
- **File naming**: `module_name.c` / `module_name.h`, lowercase with
  underscores. Interface headers end in `_interface.h` or `_Interface.h`
  (legacy tolerance); new files MUST use lowercase `_interface.h`.
- **No Hungarian notation** in new code. Legacy prefixes
  (`A_u8PortNo`, `Copy_u8Data`) are tolerated in unmodified MCAL files
  but MUST NOT be introduced in new modules.

**Rationale**: Three inconsistent naming styles currently coexist in
the codebase, increasing cognitive load. A converged convention
reduces onboarding time and merge friction.

### VII. Living Documentation

Documentation MUST be treated as a deliverable, not an afterthought:

- The project README MUST accurately reflect the current feature set,
  build instructions, memory map, and hardware target.
- Every MCAL and Service module MUST have a brief header-comment block
  stating purpose, dependencies, and usage.
- The EEPROM memory map, peripheral pin assignments, and alarm chain
  flow MUST be documented in dedicated files and updated whenever the
  implementation changes.
- Hardware documentation (schematics, BOM, assembly guide) MUST be
  version-tagged and kept in sync with firmware changes that alter pin
  usage or peripheral configuration.
- Orphaned or placeholder documentation files (empty READMEs, stale
  migration scripts) MUST be removed.

**Rationale**: The hardware documentation is already exemplary; the
software documentation has gaps and stale content. Parity across both
ensures maintainability during the upcoming enhancement phase.

## Technical Constraints & Standards

### Target Platform

- **Current MCU**: ATmega128 @ 16 MHz (128 KB Flash, 4 KB SRAM,
  4 KB EEPROM)
- **Toolchain**: avr-gcc, avr-objcopy, avrdude
- **Migration candidates under evaluation**: STM32F4xx, ESP32
- **Language**: C99 (no C++ unless migrating to a platform that
  benefits from it; any such decision requires a constitution
  amendment)

### Memory Budget

- Flash usage MUST NOT exceed 80% of target capacity to leave room for
  bootloader and future features.
- RAM usage (static + worst-case stack) MUST NOT exceed 75% of
  available SRAM. Stack depth MUST be analyzed or measured for
  interrupt-heavy paths.
- EEPROM regions MUST be defined with explicit start addresses and
  maximum sizes in `eeprom_map.h`. Region overlap MUST be caught by
  compile-time `_Static_assert` or equivalent guard.

### Build System

- The root `Makefile` is the authoritative build system.
- Build MUST support at least three targets: `all` (release), `debug`
  (with logging enabled), and `test` (host-side unit tests).
- Compiler warnings (`-Wall -Wextra -Werror` for new modules) MUST
  be enabled. Legacy modules MAY use `-Wall` without `-Werror` until
  warnings are resolved.

### EEPROM Wear Management

- Modules writing to EEPROM periodically (data logger, error log)
  MUST document their worst-case write frequency and the resulting
  cycle count over the expected device lifetime.
- Write-leveling or change-detection (skip write if value unchanged)
  SHOULD be implemented for high-frequency writers.

### Communication Protocols

- UART0: LCD interface @ 9600 baud (TopWay UART LCD)
- UART1: GSM module (SIM800L) @ 9600 baud
- SPI/I2C: Reserved for future sensor expansion
- SMS messages MUST use a retry mechanism with configurable attempt
  count and back-off delay.

## Development Workflow & Quality Gates

### Branching Strategy

- `main` branch MUST always be in a buildable, flash-ready state.
- Feature work MUST happen on dedicated branches named
  `feature/<short-description>` or `fix/<short-description>`.
- Branches MUST be rebased or squash-merged to keep linear history.

### Quality Gates (pre-merge)

1. **Build gate**: `make all` and `make debug` MUST succeed with zero
   errors.
2. **Test gate**: `make test` MUST pass all host-side unit tests.
3. **Warning gate**: No new compiler warnings introduced (checked via
   CI or manual review).
4. **Documentation gate**: If a change alters public API, memory map,
   or pin assignments, the corresponding documentation MUST be updated
   in the same commit or PR.
5. **Constitution compliance**: Changes MUST NOT violate any Core
   Principle. Violations MUST be explicitly justified in the PR
   description and approved before merge.

### Code Review Standards

- Every PR MUST be reviewed for layer-boundary violations (Principle I)
  and duplication (Principle II).
- Reviewer MUST verify that new EEPROM usage does not overlap existing
  regions (Principle II, III).
- Safety-critical changes (threshold logic, alarm chain, watchdog
  configuration) require explicit sign-off noting the safety
  implications.

### Cleanup Discipline

- Legacy code that is superseded MUST be deleted, not commented out.
  Git history preserves old versions.
- Migration and one-time scripts (e.g., `organize-files.sh`,
  `update_includes.sh`) MUST be moved to an `archive/` directory or
  removed once their purpose is fulfilled.

## Governance

This constitution is the supreme governance document for the iDMS
project. All development practices, code reviews, and architectural
decisions MUST comply with its principles.

- **Amendments**: Any change to a Core Principle requires:
  1. A written proposal describing the change and its rationale.
  2. An impact analysis on existing code and documentation.
  3. A migration plan if the change invalidates existing work.
  4. Update of this constitution file with a version increment.
- **Versioning**: The constitution follows Semantic Versioning:
  - MAJOR: Principle removal, redefinition, or backward-incompatible
    governance change.
  - MINOR: New principle or section added, or material expansion of
    existing guidance.
  - PATCH: Clarifications, typo fixes, non-semantic refinements.
- **Compliance reviews**: At the start of each major feature or
  enhancement cycle, the constitution MUST be re-read and the
  plan's "Constitution Check" section MUST enumerate how each
  principle is satisfied or explicitly waived.
- **Runtime guidance**: See `.specify/templates/agent-file-template.md`
  for development guidelines that operationalize these principles in
  day-to-day coding.

**Version**: 1.0.0 | **Ratified**: 2025-02-14 | **Last Amended**: 2025-02-14
