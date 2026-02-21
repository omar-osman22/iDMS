# Feature Specification: iDMS Foundation Overhaul

**Feature Branch**: `001-foundation-overhaul`
**Created**: 2026-02-14
**Status**: Draft

> **Note (C1)**: The branch name `001-foundation-overhaul` follows the speckit numbering convention (`NNN-short-name`) rather than the constitution's `feature/<short-description>` pattern. The constitution branching strategy (§ Development Workflow) will be amended in a follow-up patch to recognize speckit-managed branches as compliant.
**Input**: User description: "This repo is kind of old one I don't remember a lot about its state, you will find some documentations, the target of the project is to have an industrial digital measurement system iDMS, that interfaces with some sensors (temp, voltage, current) and gsm module to send warning messages to saved contacts that what I remember, also a research was made for the hardware phase and PCB design. What I will be doing next is maintaining and enhancement for the project so I need a solid base and proper contextual background, so we need to do a comprehensive research on the iDMS concept, preferred targets — considering moving from ATmega, ideas to enhance, rearrangement and cleanup for the current working directory along with updating the project documentation."

## Clarifications

### Session 2026-02-21

- Q: What is the primary deliverable format for the HW wiring user story? → A: Consolidated wiring reference card (pin-map table cross-referenced with SW config, plus connector pinouts)
- Q: Should the story fix SW/HW pin config mismatches or only document them? → A: Fix all SW/HW pin config mismatches found during cross-referencing as part of this story
- Q: Should the wiring card include power rail verification steps? → A: Include power rail verification steps with expected voltage ranges (go/no-go checks)
- Q: Which programming/flashing method should the wiring card cover? → A: ISP only (USBasp/AVR ISP mkII via 6-pin J8 header)
- Q: What defines "HW up and running" for the acceptance test? → A: Live sensor readings displayed on LCD (confirms full analog + display chain)

## User Scenarios & Testing *(mandatory)*

### User Story 1 — Repository Cleanup & De-duplication (Priority: P1)

As the project maintainer returning to the codebase after a long break, I need to remove all duplicate, orphaned, and stale files so that every file in the repository has a single, clear purpose and I can confidently navigate and build the project.

**Why this priority**: The codebase currently contains two parallel implementations of the application layer (`Application/` vs root-level `inc/` + `src/`), two `main.c` files, unused utility modules (`LinkedList.*`), empty placeholder READMEs, stale Eclipse-generated build artifacts, and one-time migration scripts. Until these are resolved, any enhancement work risks modifying the wrong file or relying on dead code. This is the foundational prerequisite for all other stories.

**Independent Test**: After completing this story, no two files implement the same module or function. Running `find . -name '*.c' -o -name '*.h' | sort` shows zero filename collisions across directories. File count is reduced by ≥5 vs baseline. Build success (`make all` with zero errors) is validated after US2 completes, since US1 alone does not fix pre-existing struct mismatch errors.

**Acceptance Scenarios**:

1. **Given** the current repository with duplicate `Application/` and root-level `inc/src/` directories, **When** the cleanup is complete, **Then** only the enhanced (root-level) implementation remains and the `Application/` directory is deleted.
2. **Given** two `main.c` files (root and `Application/main.c`), **When** cleanup is complete, **Then** only the enhanced root-level `main.c` exists.
3. **Given** unused `Utilities/LinkedList.c` and `Utilities/LinkedList.h`, **When** cleanup is complete, **Then** these files are removed from the repository.
4. **Given** placeholder files `IDMS/README.md` and `Debug/README.md` (containing only `# IDMS`), **When** cleanup is complete, **Then** these files are deleted.
5. **Given** one-time migration scripts `organize-files.sh` and `update_includes.sh`, **When** cleanup is complete, **Then** they are either moved to an `archive/` directory or deleted.
6. **Given** stale Eclipse-generated build files in `Debug/` (`subdir.mk`, `sources.mk`, `objects.mk`) that reference a flat pre-reorganization structure, **When** cleanup is complete, **Then** these files are either updated to match the current directory layout or removed in favor of the root `Makefile` as the sole build system.
7. **Given** `debug_log.c` and `debug_log.h` are source code mixed into the `Debug/` build-output directory, **When** cleanup is complete, **Then** these files are relocated to an appropriate source directory (e.g., `Service/` or a new `Debug/src/` with clear separation from build artifacts).

---

### User Story 2 — Resolve Known Technical Defects (Priority: P2)

As the project maintainer, I need all known bugs and inconsistencies in the codebase fixed so that the firmware can compile cleanly and run without data corruption.

**Why this priority**: Multiple defects have been identified that would cause linker errors, silent data corruption, or undefined behavior. These must be resolved before any new features can be reliably developed or tested.

**Independent Test**: The project compiles with zero errors and zero new warnings. EEPROM regions are provably non-overlapping. All modules that call EEPROM functions link successfully against a single consistent API.

**Acceptance Scenarios**:

1. **Given** the EEPROM header declares `EEPROM_WriteByte`/`EEPROM_ReadByte` but callers use `EEPROM_voidWriteData`/`EEPROM_u8ReadData`, **When** the defect is resolved, **Then** a single, consistent EEPROM API exists and all modules compile and link cleanly against it.
2. **Given** the data logger at EEPROM address `0x300` (50 entries × ~17 bytes = ~856 bytes) overlaps with the error log at `0x400`, **When** the defect is resolved, **Then** all EEPROM regions have non-overlapping address ranges defined in a single shared header, and a compile-time guard prevents future overlaps.
3. **Given** `static AppState appState = {0}` is declared in the header file (producing a separate copy in every translation unit that includes it), **When** the defect is resolved, **Then** `appState` is defined in the `.c` file and accessed via extern declaration or getter functions.
4. **Given** the `VREF` macro is defined identically in both `CTcurrent_config.h` and `PT100_config.h`, **When** the defect is resolved, **Then** `VREF` is defined in exactly one shared header and included by both sensor configs.
5. **Given** `PhoneList.c` uses `avr/eeprom.h` directly while other modules use the project's EEPROM driver, **When** the defect is resolved, **Then** all modules use the same EEPROM abstraction consistently.
6. **Given** the main loop uses a bare `for` loop as a delay (`for(u32 i = 0; i < 1000000; i++);`), **When** the defect is resolved, **Then** the delay uses a reliable mechanism that is not subject to compiler optimization removal.

---

### User Story 3 — Unified Build System (Priority: P3)

As the project maintainer, I need a single, well-structured build system so that I can build, test, and flash the firmware with clear, documented commands.

**Why this priority**: Two incompatible build systems coexist (root `Makefile` building `main.elf` and Eclipse-generated `Debug/makefile` building `CodeCrafters.elf`). This causes confusion about which build to trust and makes CI/CD setup impossible.

**Independent Test**: Running `make all`, `make debug`, and `make test` from the repository root produces the expected outputs. No other build system exists in the project (or if kept for IDE support, it is clearly documented as secondary and auto-generated).

**Acceptance Scenarios**:

1. **Given** two incompatible build systems in the repository, **When** unification is complete, **Then** the root `Makefile` is the single authoritative build system and all legacy build files are removed or clearly marked as IDE-generated artifacts.
2. **Given** the root `Makefile`, **When** `make all` is run, **Then** it produces a flashable binary from the current organized directory structure with zero errors.
3. **Given** the root `Makefile`, **When** `make test` is run, **Then** it compiles and executes host-side unit tests (even if initially there are zero tests — the target must exist and succeed).
4. **Given** the root `Makefile`, **When** `make clean` is run, **Then** all build artifacts are removed without affecting source files or documentation.

---

### User Story 4 — MCU Migration Research & Evaluation (Priority: P4)

As the project maintainer planning future enhancements, I need a documented comparison of the current ATmega128 platform against candidate MCUs (STM32F4xx, ESP32, and others) so that I can make an informed decision about whether and when to migrate.

**Why this priority**: The ATmega128 is at 54% RAM usage with current features. Future enhancements (WiFi, cloud, more sensors) will exceed its capacity. However, migration is a large effort that must be justified. This story produces a decision document, not a code change.

**Independent Test**: A research document exists in the specs directory that compares at least three MCU candidates across defined criteria. The document concludes with a recommendation and a rough migration effort estimate.

**Acceptance Scenarios**:

1. **Given** the current system runs on ATmega128 (128 KB Flash, 4 KB SRAM, 4 KB EEPROM), **When** the research is complete, **Then** a document compares the ATmega128 against at least two alternative MCU families on: processing power, memory (Flash/SRAM/EEPROM-equivalent), built-in communication peripherals (WiFi, BLE, USB), power consumption, toolchain maturity, cost, and community/ecosystem size.
2. **Given** each candidate MCU, **When** the research is complete, **Then** the document maps each iDMS feature (sensor ADC, UART LCD, UART GSM, EEPROM storage, GPIO control) to the equivalent peripheral or approach on the candidate MCU.
3. **Given** the comparison document, **When** reviewed, **Then** it includes a recommended target with justification, a list of MCAL modules that would need to be rewritten, and an estimated effort level (small/medium/large) for the migration.

---

### User Story 5 — Updated Project Documentation (Priority: P5)

As the project maintainer (or a new contributor), I need the project documentation to accurately describe the current state of the system — its features, architecture, build instructions, memory map, and hardware — so that I can understand and contribute to the project without guessing.

**Why this priority**: The README and supporting docs contain stale references (e.g., "CodeCrafters" project name, outdated directory references, pre-enhancement API listings). Accurate documentation is required by Constitution Principle VII and is essential for onboarding or resuming work.

**Independent Test**: A new contributor can read the README and successfully clone, build, flash, and run basic tests by following only the documented instructions. All documented API functions correspond to functions that actually exist in the code. The EEPROM memory map in documentation matches the one defined in the code header.

**Acceptance Scenarios**:

1. **Given** the current README references features, build instructions, and directory structures, **When** the documentation update is complete, **Then** every section in the README accurately reflects the post-cleanup state of the repository.
2. **Given** the EEPROM memory map is described in the README, **When** the documentation update is complete, **Then** the documented map matches the authoritative `eeprom_map.h` header exactly.
3. **Given** the API Reference section lists function signatures, **When** the documentation update is complete, **Then** every listed function exists in the codebase and every public function in the codebase is listed.
4. **Given** the hardware documentation in `Hardware/` is thorough, **When** the documentation update is complete, **Then** pin assignments and peripheral configurations mentioned in hardware docs are consistent with the MCAL `_config.h` files.
5. **Given** the project historically used the name "CodeCrafters", **When** the documentation update is complete, **Then** no references to "CodeCrafters" remain in documentation or build output names.

---

### User Story 6 — Enhancement Ideas Backlog (Priority: P6)

As the project maintainer planning the next development cycle, I need a prioritized list of enhancement ideas grounded in the research and cleanup findings so that I can plan future feature branches effectively.

**Why this priority**: This is a planning deliverable, not a code change. It depends on the outcomes of the cleanup (US1-US3) and the migration research (US4) to propose realistic enhancements. It provides the roadmap for subsequent `/speckit.specify` calls.

**Independent Test**: A document exists listing at least 8 enhancement ideas, each with a brief description, estimated complexity, dependency on MCU migration (yes/no), and suggested priority.

**Acceptance Scenarios**:

1. **Given** the completed cleanup and migration research, **When** the backlog is created, **Then** it lists ideas in categories (reliability, connectivity, user interface, maintainability, testing) with at least two ideas per category.
2. **Given** each enhancement idea, **When** reviewed, **Then** it specifies whether it can be done on the current ATmega128, requires the new MCU, or is platform-independent.
3. **Given** the enhancement backlog, **When** reviewed, **Then** it is ordered by suggested implementation priority, considering dependencies between enhancements.

---

### User Story 7 — Hardware Wiring Reference & Pin Configuration (Priority: P3)

As the project maintainer setting up the hardware for deployment and testing, I need a consolidated wiring reference card that cross-references every hardware connector and pin with its software configuration, so that I can wire the system correctly on the first attempt and verify it is fully operational.

**Why this priority**: The existing hardware documentation (Hardware/*.md) is thorough but spread across multiple files, and critically, the software pin configurations in MCAL (e.g., DIO_config.h sets Port A as OUTPUT while hardware docs and ADC usage require PA0/PA1 as analog inputs) contain mismatches that would cause wiring failures. A single authoritative reference card that matches the actual firmware eliminates trial-and-error during bench setup.

**Independent Test**: The wiring reference card exists with a pin-map table covering all active MCU pins. Every pin direction/function in the card matches the corresponding MCAL config header. Following the card from power-on through ISP flash results in live sensor readings on the LCD.

**Acceptance Scenarios**:

1. **Given** the hardware documentation spread across multiple files and SW pin configs in MCAL headers, **When** the wiring reference card is complete, **Then** a single document contains: (a) a pin-map table for all active ATmega128 pins cross-referenced with the SW config macro and Hardware/*.md connector, (b) connector pinout diagrams for J1–J8, and (c) power rail verification steps with expected voltage ranges.
2. **Given** DIO_config.h sets all Port A pins as `DIO_OUTPUT` while PA0 and PA1 must be analog inputs for PT100 and CT sensors, **When** this story is complete, **Then** all SW/HW pin configuration mismatches are fixed in the MCAL config headers to match the hardware design intent.
3. **Given** the ISP programming header J8 is documented in the Assembly Guide, **When** the wiring card is complete, **Then** the ISP connection (USBasp via 6-pin J8) is documented with pin assignments (MISO, MOSI, SCK, RST, VCC, GND) matching the actual PCB layout.
4. **Given** a correctly wired system, **When** the user follows the power-on verification sequence in the card (measure each rail: +5V digital, +3.3V logic, +5V analog, +3.7V GSM), **Then** each rail reads within its documented tolerance before any module is connected.
5. **Given** the system is wired and flashed per the reference card, **When** power is applied with sensors connected, **Then** live sensor readings (temperature and current) are displayed on the TopWay LCD, confirming the full signal chain (power → sensor → ADC → MCU → UART → LCD) is operational.

---

### Edge Cases

- What happens if the cleanup removes a file that is actually still used by a non-obvious part of the build? A pre-cleanup build verification and post-cleanup build verification must both succeed for the cleanup to be accepted.
- What if the EEPROM address fix requires changing existing stored data layout? A migration strategy must be documented for devices that already have data stored at old addresses (or this is deferred if no devices are deployed).
- What if the stale `Debug/` build system is needed for a specific IDE (Atmel Studio, Eclipse)? Document the IDE's project file generation process so developers can regenerate configs on demand rather than storing stale ones in version control.
- What if a pin configuration mismatch in DIO_config.h causes a peripheral to malfunction on the bench? The wiring reference card includes go/no-go voltage checks at each power rail before connecting modules, and the SW fixes are validated by confirming live sensor readings on the LCD.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The repository MUST contain no duplicate implementations of the same module (no two `app.c`, no two `main.c`, no two linked-list implementations).
- **FR-002**: The EEPROM memory map MUST be defined in a single header file with compile-time overlap guards for all defined regions.
- **FR-003**: All modules that interact with EEPROM MUST use the same EEPROM abstraction layer; direct hardware library calls (`avr/eeprom.h`) outside the MCAL driver MUST NOT exist in Service or Application code.
- **FR-004**: The root `Makefile` MUST successfully build the project from the current organized directory structure and expose `all`, `debug`, `test`, and `clean` targets.
- **FR-005**: A MCU comparison research document MUST evaluate at least two alternative MCU families against the current ATmega128 using defined criteria.
- **FR-006**: The project README MUST accurately describe features, build instructions, directory structure, EEPROM map, and API functions as they exist in the codebase at the time of documentation.
- **FR-007**: New and modified code MUST compile with `-Wall -Wextra` and zero warnings. The total warning count MUST NOT increase above the pre-cleanup baseline (currently 29). Legacy warnings in unmodified files are tolerated per constitution (§ Build System).
- **FR-008**: Shared constants (`VREF`, EEPROM base addresses) MUST be defined in exactly one location and included where needed.
- **FR-009**: The `AppState` variable MUST NOT be declared as `static` in a header file; it MUST be defined in a single `.c` file.
- **FR-010**: A prioritized enhancement backlog document MUST be produced, containing at least 8 ideas categorized by theme and annotated with platform dependency.
- **FR-011**: A consolidated wiring reference card MUST exist as a single document containing: a pin-map table for all active ATmega128 pins cross-referenced with the SW config macro (`DIO_config.h`, `ADC_register.h`, `UART_Registers.h`) and Hardware/*.md connector designator, connector pinout diagrams (J1–J8), power rail verification steps with expected voltage ranges, and ISP programming connection (USBasp via 6-pin J8).
- **FR-012**: All MCAL pin configuration headers (`DIO_config.h` and related) MUST match the hardware design intent documented in `Hardware/Hardware_Integration.md` — specifically, ADC input pins (PA0, PA1) MUST NOT be configured as digital outputs, and UART pins MUST have correct direction settings.

### Key Entities

- **EEPROM Memory Region**: A named, bounded address range within the MCU's EEPROM, with a defined start address, maximum byte size, owning module, and write-frequency estimate. Regions must not overlap.
- **MCU Candidate**: A microcontroller platform being evaluated for migration, characterized by processing specs, memory sizes, built-in peripherals, toolchain, cost, and feature-mapping to iDMS requirements.
- **Enhancement Idea**: A proposed improvement to the system, with a title, description, category, platform dependency (current MCU / new MCU / platform-independent), estimated complexity, and suggested priority rank.
- **Wiring Reference Card**: A single-page (or minimal-page) document mapping every active MCU pin to its hardware connector, signal name, SW config macro, and direction. Includes power rail go/no-go voltage checks and ISP programming pinout.

## Assumptions

- The enhanced root-level `inc/app.h` + `src/app.c` + `main.c` represent the current, intended implementation. The older `Application/` directory contents are superseded and safe to delete.
- No physical devices have been deployed with the current EEPROM layout, so the address map can be changed without a data-migration strategy for existing devices.
- The Eclipse/Atmel Studio IDE project (Debug/ build system) is not actively used and can be removed. If IDE support is needed in the future, project files can be regenerated.
- The hardware documentation in `Hardware/` is accurate and current; it does not require a full rewrite. However, cross-referencing against firmware config files has revealed pin configuration mismatches (e.g., Port A direction in DIO_config.h) that will be fixed as part of US7.
- "Voltage" monitoring mentioned by the user refers to the existing power calculation (current × 220V source voltage) rather than a separate voltage-sensing channel. The system does not currently have a dedicated voltage sensor — it infers voltage from the known source.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: After cleanup, `make all` produces a flashable binary with zero errors and no increase in compiler warnings compared to the pre-cleanup baseline.
- **SC-002**: Running `find . -name '*.c' -o -name '*.h' | xargs grep -l 'EEPROM'` shows all EEPROM-using modules reference the same API — no direct `avr/eeprom.h` usage outside MCAL.
- **SC-003**: A new contributor can clone the repository, read the README, and successfully build the project within 10 minutes using only the documented instructions.
- **SC-004**: The MCU comparison document covers at least 3 platforms and provides a recommendation with estimated migration effort per MCAL module (Small/Medium/Large) and a total timeline range (e.g., 2–3 weeks), enabling a go/no-go migration decision.
- **SC-005**: The enhancement backlog contains at least 8 ideas spanning at least 3 categories, each annotated with platform dependency and relative priority.
- **SC-006**: All EEPROM regions defined in the code fit within the 4 KB address space without overlap, verified by compile-time assertions or a documented manual check.
- **SC-007**: Zero references to "CodeCrafters" remain in documentation or build configuration files.
- **SC-008**: The repository file count is reduced by at least 5 files (duplicates/placeholders removed) compared to the pre-cleanup baseline.
- **SC-009**: The wiring reference card contains a pin-map table for all active ATmega128 pins, and every pin's direction/function in the card matches the corresponding MCAL config header (`DIO_config.h`) — verified by manual cross-check or automated grep.
- **SC-010**: Following the wiring reference card from power-on (with rail voltage verification) through ISP flash results in live sensor readings displayed on the TopWay LCD, confirming the full signal chain is operational.
