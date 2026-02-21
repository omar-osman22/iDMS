# Tasks: iDMS Foundation Overhaul

**Input**: Design documents from `/specs/001-foundation-overhaul/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/

**Tests**: Constitution Principle V requires boundary-condition and nominal-path tests for new/modified modules. Minimal test tasks are included in Phase 2 and Phase 4. Full test suite expansion is deferred to a follow-up feature.

**Output Locations**: Deliverable documents for US4 and US6 are placed under `docs/` at the repository root (not under `specs/`), since they are project-level artifacts intended for stakeholder review.

**Organization**: Tasks are grouped by user story (P1–P6 + US7 at P3) to enable independent implementation and verification of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2)
- Exact file paths included in all descriptions

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Snapshot the current state so cleanup can be verified and rolled back if needed.

- [x] T001 Run `make all 2>&1 | tail -20` and save output to specs/001-foundation-overhaul/baseline-build.log
- [x] T002 Run `find . -name '*.c' -o -name '*.h' | grep -v specs | grep -v .specify | sort > specs/001-foundation-overhaul/baseline-files.txt` to capture file inventory
- [x] T003 Create git commit on branch `001-foundation-overhaul` with message "chore: baseline snapshot before foundation overhaul"

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Create the new shared headers that multiple user stories depend on. These MUST exist before US1 cleanup or US2 defect fixes can proceed.

**⚠️ CRITICAL**: No user story work can begin until this phase is complete.

- [x] T004 Create Config/eeprom_map.h with region macros and `_Static_assert` overlap guards per contracts/eeprom_map.h
- [x] T005 [P] Create Config/sensor_common.h with VREF, ADC_RESOLUTION, SOURCE_VOLTAGE, SYSTEM_TICK_MS per contracts/sensor_common.h
- [x] T006 [P] Create MCAL/Delay/delay.h with MCAL_DelayMs/MCAL_DelayUs macros and TARGET_AVR/TARGET_HOST guards per contracts/delay_shim.h
- [x] T007 Add `#define` aliases (`EEPROM_voidWriteData`, `EEPROM_u8ReadData`) and `EEPROM_WriteBlock`/`EEPROM_ReadBlock` declarations to MCAL/EEPROM/INTERNAL_EEPROM.h per contracts/eeprom_api.h
- [x] T008 Implement `EEPROM_WriteBlock` and `EEPROM_ReadBlock` in MCAL/EEPROM/INTERNAL_EEPROM.c
- [x] T009 Verify `make all` compiles (expected: still broken from pre-existing app.c struct errors, but no new errors introduced by T004–T008)
- [x] T010 [P] Add brief header-comment block (purpose, dependencies, usage) to Config/eeprom_map.h, Config/sensor_common.h, and MCAL/Delay/delay.h per Constitution Principle VII
- [x] T011 Write host-side boundary test for EEPROM map overlap guards: compile Config/eeprom_map.h with `gcc -DTARGET_HOST -fsyntax-only` and verify `_Static_assert` passes; intentionally break one guard and verify compile failure

**Checkpoint**: Foundational headers exist and are tested. User story implementation can now begin.

---

## Phase 3: User Story 1 — Repository Cleanup & De-duplication (Priority: P1) 🎯 MVP

**Goal**: Remove all duplicate, orphaned, and stale files so every file has a single clear purpose.

**Independent Test**: No two files implement the same module. `find . -name '*.c' -o -name '*.h' | sort` shows zero filename collisions. File count reduced by ≥5. Build success is validated after US2 (US1 alone does not fix pre-existing struct mismatch errors).

### Implementation for User Story 1

- [x] T012 [US1] Delete Application/main.c, Application/inc/app.h, Application/src/app.c, and remove the empty Application/ directory tree
- [x] T013 [P] [US1] Delete Utilities/LinkedList.c and Utilities/LinkedList.h
- [x] T014 [P] [US1] Delete placeholder files IDMS/README.md and Debug/README.md, then remove empty IDMS/ directory
- [x] T015 [P] [US1] Delete one-time migration scripts organize-files.sh and update_includes.sh
- [x] T016 [US1] Relocate Debug/debug_log.c to Service/debug_log.c and Debug/debug_log.h to Service/debug_log.h (overwrite if Service/ copies already exist; verify content is identical first)
- [x] T017 [US1] Delete stale Eclipse build files: Debug/makefile, Debug/subdir.mk, Debug/sources.mk, Debug/objects.mk
- [x] T018 [P] [US1] Delete build artifacts: Debug/CodeCrafters.lss and all Debug/*.d files (ADC_Program.d, DIO.d, INTERNAL_EEPROM.d, LM35_Program.d, main.d, PhoneList.d, SIM_prog.d, TopWayLCD_Program.d, UART_Prog.d)
- [x] T019 [US1] Verify file count reduced by ≥5 vs baseline: `find . -name '*.c' -o -name '*.h' | grep -v specs | grep -v .specify | wc -l`
- [x] T020 [US1] Git commit with message "feat(US1): remove duplicate and orphaned files"

**Checkpoint**: Repository is clean. Only canonical files remain.

---

## Phase 4: User Story 2 — Resolve Known Technical Defects (Priority: P2)

**Goal**: Fix all known bugs and inconsistencies so the firmware compiles cleanly and runs without data corruption.

**Independent Test**: `make all` produces zero errors. All EEPROM modules reference a single API. No `avr/` headers appear in Service/ or src/.

### Implementation for User Story 2

- [x] T021 [US2] Update MCAL/EEPROM/INTERNAL_EEPROM.h to include Config/eeprom_map.h and add the `#define` aliases if not already done in T007 (verify integration)
- [x] T022 [US2] Update Service/data_logger.c to include Config/eeprom_map.h and use EEPROM_DATA_LOG_BASE instead of hardcoded 0x300
- [x] T023 [P] [US2] Update Service/error_handler.c to include Config/eeprom_map.h and use EEPROM_ERROR_LOG_BASE instead of hardcoded 0x400
- [x] T024 [US2] Migrate Service/PhoneList.c from `avr/eeprom.h` to use MCAL/EEPROM/INTERNAL_EEPROM.h API (replace eeprom_write_byte/eeprom_read_byte with EEPROM_WriteByte/EEPROM_ReadByte, and eeprom_write_block/eeprom_read_block with EEPROM_WriteBlock/EEPROM_ReadBlock)
- [x] T025 [US2] Move `static AppState appState = {0}` from inc/app.h to src/app.c, replace with `extern AppState appState;` declaration in inc/app.h
- [x] T026 [US2] Fix SensorReading struct in inc/app.h to include all 9 fields per data-model.md (rawValue, filteredValue, prevFiltered, variance, mean, sampleCount, isCalibrated, minValue, maxValue) — this resolves the 8 build errors
- [x] T027 [P] [US2] Remove `#define VREF` from Service/CTcurrent_config.h and Service/PT100_config.h, replace with `#include "Config/sensor_common.h"` in both files
- [x] T028 [US2] Replace `#include <avr/delay.h>` in Service/TopWayLCD_Program.c with `#include "MCAL/Delay/delay.h"` and replace `_delay_ms()` calls with `MCAL_DelayMs()`
- [x] T029 [P] [US2] Replace `#include <avr/delay.h>` in Service/SIM_prog.c with `#include "MCAL/Delay/delay.h"` and replace `_delay_ms()` calls with `MCAL_DelayMs()`
- [x] T030 [US2] Replace the bare `for(u32 i = 0; i < 1000000; i++);` delay loop in main.c with `MCAL_DelayMs()` call and add `#include "MCAL/Delay/delay.h"`
- [x] T031 [US2] Fix sendAlarmMessage() mixed-UART bug in src/app.c — ensure all SMS AT commands and payload data use UART0 functions consistently (replace UART_voidWriteData calls with UART_SendStringSync or equivalent UART0 function); see research.md §9 for details
- [x] T032 [US2] Verify `make all` compiles with zero errors: `make all 2>&1 | grep -c error` returns 0
- [x] T033 [US2] Count remaining compiler warnings: `make all 2>&1 | grep -c warning` — document result; target: no increase vs baseline (≤29). New/modified files MUST have zero warnings with `-Wall -Wextra`
- [x] T034 [P] [US2] Add brief header-comment block to all modified MCAL and Service files (INTERNAL_EEPROM.h, PhoneList.c, data_logger.c, error_handler.c, TopWayLCD_Program.c, SIM_prog.c) per Constitution Principle VII
- [x] T035 [US2] Write host-side smoke test: compile MCAL/EEPROM/INTERNAL_EEPROM.h + Config/eeprom_map.h + Config/sensor_common.h with `gcc -DTARGET_HOST -fsyntax-only` — must succeed with zero errors
- [x] T036 [US2] Git commit with message "fix(US2): resolve EEPROM overlap, API mismatch, static-in-header, avr header leaks, and mixed-UART bug"

**Checkpoint**: Firmware compiles cleanly. All known defects are resolved.

---

## Phase 5: User Story 3 — Unified Build System (Priority: P3)

**Goal**: Single, well-structured Makefile with `all`, `debug`, `test`, and `clean` targets.

**Independent Test**: `make all`, `make debug`, `make test`, and `make clean` all succeed from the repository root. No other build system exists.

### Implementation for User Story 3

- [x] T037 [US3] Update SRC_DIRS in Makefile to: `./src ./Service ./MCAL/ADC ./MCAL/DIO ./MCAL/UART ./MCAL/EEPROM ./Config`
- [x] T038 [US3] Update SRCS in Makefile to explicitly include root main.c: `SRCS = main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))`
- [x] T039 [US3] Update INCLUDE_DIRS in Makefile to include `./inc ./Config ./MCAL/Delay` and remove `./Application ./Application/inc`
- [x] T040 [US3] Add/verify `debug` target in Makefile with `-DDEBUG_ENABLE -g -O0` flags
- [x] T041 [US3] Add `test` target in Makefile that compiles with host `gcc` and `-DTARGET_HOST` flag — initially a no-op runner that returns 0 (placeholder for Unity integration)
- [x] T042 [US3] Add `clean` target in Makefile that removes the build/ output directory
- [x] T043 [US3] Verify all four targets work: `make clean && make all && make debug && make test`
- [x] T044 [US3] Git commit with message "feat(US3): unify build system with all/debug/test/clean targets"

**Checkpoint**: Build system is unified and fully functional.

---

## Phase 6: User Story 7 — Hardware Wiring Reference & Pin Configuration (Priority: P3)

**Goal**: Fix all DIO pin configuration mismatches, correct UART documentation labels, and produce a consolidated wiring reference card that enables first-attempt bench setup.

**Independent Test**: The wiring reference card exists at Hardware/Wiring_Reference.md with a pin-map table covering all active MCU pins. Every pin direction/function in the card matches MCAL/DIO/DIO_config.h. Power rail go/no-go voltage ranges are documented. Following the card from power-on through ISP flash results in live sensor readings on the LCD.

**Note**: US7 touches different files than US1–US3 (DIO_config.h, DIO_program.c, Hardware/*.md) and can run in parallel with those stories after Phase 2. It is placed here in priority order (P3, same as US3).

### Implementation for User Story 7

- [x] T045 [US7] Update MCAL/DIO/DIO_config.h with corrected pin directions for all 7 ports (A–G) per contracts/dio_config_corrected.h — fix Port A all to DIO_INPUT (ADC), Port B PB0/PB3 to DIO_INPUT (ISP), Port C PC0/PC2/PC3 to DIO_OUTPUT (GSM reset, relays), Port D PD1 to DIO_OUTPUT (UART0 TX), and add new Port E/F/G sections with inline comments
- [x] T046 [US7] Uncomment and verify MDIO_vInt() initialization function in MCAL/DIO/DIO_program.c — ensure it applies DIO_config.h direction macros to DDR registers for all 7 ports (A–G); currently commented out so pin directions are never set at runtime
- [x] T047 [P] [US7] Correct UART peripheral labels in Hardware/Hardware_Integration.md — change all UART0→LCD references to UART0→GSM(SIM800L) and UART1→GSM references to UART1→LCD(TopWay) to match actual firmware register usage per research.md §9
- [x] T048 [P] [US7] Correct UART peripheral labels in Hardware/PCB_Schematic.md — update net labels to UART0(PD0/PD1)→GSM and UART1(PE0/PE1)→LCD per actual code
- [x] T049 [US7] Create Hardware/Wiring_Reference.md with consolidated pin-map table — for every active ATmega128 pin across ports A–G, list: port/pin, function, direction (I/O), SW config macro (DIO_PIN_Xn_DIR / ADC channel / UART register), and HW connector reference (J1–J8) per data-model.md §6 and §8
- [x] T050 [US7] Add connector pinout section to Hardware/Wiring_Reference.md — J1 through J8 pinouts per data-model.md §8 with pin assignments, signal names, and connected modules (J1=power, J2=PT100, J3=CT, J4=LCD/UART1, J5=GSM/UART0, J6=antenna, J7=SIM, J8=ISP)
- [x] T051 [US7] Add ISP programming section to Hardware/Wiring_Reference.md — J8 2×3 header pinout (MISO/PB3, VCC, SCK/PB1, MOSI/PB2, RST, GND), USBasp connection procedure, and avrdude flash command (`avrdude -c usbasp -p m128 -U flash:w:build/main.hex:i`)
- [x] T052 [US7] Add power rail verification section to Hardware/Wiring_Reference.md — 5 rails (+12V input 10–15V, +5V digital 4.75–5.25V, +3.3V logic 3.2–3.4V, +5V analog 4.90–5.10V, +3.7V GSM 3.6–3.8V) with regulator ID, measurement point, and no-go action per research.md §10
- [x] T053 [US7] Add power-on verification sequence to Hardware/Wiring_Reference.md — 7-step go/no-go procedure: (1) visual inspect for shorts, (2) apply 12V and verify J1, (3) measure +5V digital, (4) measure +3.3V logic, (5) measure +5V analog, (6) measure +3.7V GSM, (7) check idle current <200 mA — connect modules only after all rails pass
- [x] T054 [US7] Verify `make all` compiles after DIO_config.h and DIO_program.c changes — zero new errors introduced
- [x] T055 [P] [US7] Add header-comment block to MCAL/DIO/DIO_config.h documenting cross-reference source (contracts/dio_config_corrected.h, Hardware_Integration.md), UART peripheral assignment note (UART0→GSM, UART1→LCD), and change history
- [x] T056 [US7] Git commit with message "feat(US7): fix DIO pin config for all ports, correct UART labels, add wiring reference card"

**Checkpoint**: DIO pin configuration matches hardware design. Wiring reference card enables first-attempt bench setup with power rail verification.

---

## Phase 7: User Story 4 — MCU Migration Research & Evaluation (Priority: P4)

**Goal**: Documented MCU comparison enabling informed migration decision.

**Independent Test**: Research document exists comparing ≥3 platforms with a recommendation and effort estimate.

### Implementation for User Story 4

- [x] T057 [US4] Verify specs/001-foundation-overhaul/research.md Section 6 (MCU Migration Comparison) is complete with comparison matrix, feature mapping, effort estimates, and recommendation
- [x] T058 [US4] Create docs/MCU_Migration_Report.md at the repository root docs/ directory — extract and format the MCU comparison from research.md into a standalone, reviewer-friendly document with: executive summary, comparison table, feature mapping, migration effort breakdown, and recommendation (STM32F411CEU6 primary, ESP32-S3 secondary)
- [x] T059 [US4] Git commit with message "docs(US4): add MCU migration research report"

**Checkpoint**: Migration decision document is ready for review.

---

## Phase 8: User Story 5 — Updated Project Documentation (Priority: P5)

**Goal**: README and supporting docs accurately reflect the post-cleanup state.

**Independent Test**: A new contributor can clone, build, and flash by following only the README. All documented APIs exist in code. No "CodeCrafters" references remain.

### Implementation for User Story 5

- [x] T060 [US5] Rewrite README.md: update project name (remove any "CodeCrafters" references), update feature list, update directory structure to match post-cleanup layout
- [x] T061 [US5] Update README.md build instructions section to match the unified Makefile targets (make all, make debug, make test, make clean, make upload)
- [x] T062 [US5] Update README.md EEPROM memory map section to match Config/eeprom_map.h (PhoneList 0x000, Config 0x200, DataLog 0x280, ErrorLog 0x680, Reserved 0x800)
- [x] T063 [US5] Update README.md API reference section — audit every listed function against actual codebase; remove functions that don't exist; add missing public functions
- [x] T064 [P] [US5] Verify pin assignments in Hardware/*.md files are consistent with MCAL DIO_config.h, ADC_register.h, and UART_Registers.h — document any remaining discrepancies (US7 should have resolved the known ones)
- [x] T065 [US5] Remove all "CodeCrafters" references from documentation and build config: `grep -rn "CodeCrafters" . --include='*.md' --include='*.mk' --include='Makefile'` must return empty
- [x] T066 [US5] Git commit with message "docs(US5): update all project documentation to reflect post-cleanup state"

**Checkpoint**: Documentation is accurate and self-consistent.

---

## Phase 9: User Story 6 — Enhancement Ideas Backlog (Priority: P6)

**Goal**: Prioritized list of ≥8 enhancement ideas grounded in cleanup findings and migration research.

**Independent Test**: Document exists with ≥8 ideas across ≥3 categories, each annotated with platform dependency and priority.

### Implementation for User Story 6

- [x] T067 [US6] Create docs/ENHANCEMENT_BACKLOG.md with categorized enhancement ideas (Reliability, Connectivity, UI, Maintainability, Testing) — at least 2 ideas per category, each with: title, description, platform dependency (ATmega128 / New MCU / Platform-independent), complexity (Low/Medium/High), suggested priority
- [x] T068 [US6] Add cross-references from enhancement backlog to relevant research.md sections and MCU migration report where applicable
- [x] T069 [US6] Git commit with message "docs(US6): add prioritized enhancement backlog"

**Checkpoint**: Enhancement roadmap is ready for planning next feature branches.

---

## Phase 10: Polish & Cross-Cutting Concerns

**Purpose**: Final validation across all user stories including US7 wiring and pin configuration.

- [ ] T070 [P] Run full build verification: `make clean && make all && make debug && make test` — all must succeed with zero errors
- [ ] T071 Verify SC-001: zero errors, no warning increase vs baseline
- [ ] T072 [P] Verify SC-002: `grep -rn 'avr/eeprom.h' Service/ src/ inc/` returns empty
- [ ] T073 Verify SC-006: `_Static_assert` guards in Config/eeprom_map.h compile without errors
- [ ] T074 [P] Verify SC-007: `grep -rn 'CodeCrafters' . --include='*.md' --include='*.mk' --include='Makefile'` returns empty
- [ ] T075 Verify SC-008: file count reduced by ≥5 vs specs/001-foundation-overhaul/baseline-files.txt
- [ ] T076 [P] Verify SC-009: cross-check Hardware/Wiring_Reference.md pin-map table against MCAL/DIO/DIO_config.h — every pin direction must match
- [ ] T077 Verify SC-010: document that wiring reference card procedure (power-on → rail verification → ISP flash → sensor readings on LCD) covers the full signal chain — create a bench-test checklist in Hardware/Wiring_Reference.md if not already present
- [ ] T078 Run quickstart.md verification checklist (all items in the Verification Checklist section, including US7-specific items: DIO_config.h checks, UART label checks, wiring card existence, power rail docs)
- [ ] T079 Final git commit with message "chore: foundation overhaul complete — all success criteria verified"

---

## Dependencies & Execution Order

### Phase Dependencies

```
Phase 1 (Setup)
    │
    ▼
Phase 2 (Foundational) ◄── BLOCKS all user stories
    │
    ├──► Phase 3 (US1: Cleanup) ──► Phase 4 (US2: Defects) ──► Phase 5 (US3: Build)
    │                                                                │
    ├──► Phase 6 (US7: HW Wiring) ── can run in parallel ◄──────────┘
    │         │                                                      │
    │         ├──────────────────────────────────────────────────────►│
    │         │                                                      ▼
    ├──► Phase 7 (US4: Research) ── can run in parallel      Phase 8 (US5: Docs)
    │         │                                                      │
    │         └──────────────────────────────────────────────►Phase 9 (US6: Backlog)
    │
    └──► Phase 10 (Polish) depends on ALL user stories
```

### User Story Dependencies

- **US1 (P1)**: Depends on Phase 2 only. **Must complete before US2** (US2 modifies files that US1 removes/relocates).
- **US2 (P2)**: Depends on US1 completion (clean file tree needed). **Must complete before US3** (Makefile paths depend on final file locations). Includes sendAlarmMessage() mixed-UART fix discovered during US7 research.
- **US3 (P3)**: Depends on US2 completion (all source files in final locations, all includes updated).
- **US7 (P3)**: Depends on Phase 2 only. **Can run in parallel with US1, US2, US3, US4** (touches entirely different files: DIO_config.h, DIO_program.c, Hardware/*.md). No code dependencies on other stories.
- **US4 (P4)**: Depends on Phase 2 only. **Can run in parallel with US1–US3 and US7** (documentation-only, no code changes).
- **US5 (P5)**: Depends on US1+US2+US3+US7 (docs must describe post-cleanup, post-fix, post-build-unification state, and HW docs must already have corrected UART labels from US7).
- **US6 (P6)**: Depends on US4+US5 (backlog references migration research and final project state).

### Within Each User Story

- File deletions/relocations before file modifications
- Header changes before source changes
- Config/contract files before implementation files
- Build verification after all changes in the story
- Git commit as final task per story

### Parallel Opportunities

**Phase 2 (Foundational)**:
```
T004 (eeprom_map.h)  ─┐
T005 (sensor_common.h) ┼── All [P] tasks run in parallel
T006 (delay.h)        ─┘
T007+T008 (EEPROM API) ── After T004 (needs eeprom_map.h)
```

**Phase 3 (US1 Cleanup)**:
```
T013 (LinkedList)     ─┐
T014 (placeholder MDs) ┼── All [P] deletions run in parallel
T015 (scripts)        ─┘
T012 (Application/)   ── Can also run in parallel
T016 (relocate debug_log) ── After deletions
```

**Phase 4 (US2 Defects)**:
```
T023 (error_handler)    ─┐
T027 (VREF removal)     ┼── [P] tasks on different files
T029 (SIM_prog delay)   ─┘
T034 (header comments)  ── After all modifications
```

**Phase 6 (US7 HW Wiring) — Fully parallel with US1–US3**:
```
US1─US2─US3 (sequential) ─┐
                           ├── Both tracks run after Phase 2
US7 (independent files)   ─┘

Within US7:
T047 (HW_Integration.md) ─┐
T048 (PCB_Schematic.md)   ┼── [P] different files
T055 (DIO header comment) ─┘
T045 (DIO_config.h) → T046 (DIO_program.c) → T049–T053 (Wiring_Reference.md) → T054 (verify build)
```

**Cross-Story Parallelism**:
```
After Phase 2 completes:
  Track A: US1 → US2 → US3 (sequential, code changes)
  Track B: US7 (independent, DIO + HW docs)
  Track C: US4 (independent, documentation only)
After Tracks A+B complete:
  Track D: US5 (docs depend on final state)
After Tracks C+D complete:
  Track E: US6 (backlog)
After all tracks:
  Polish (Phase 10)
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup (baseline capture)
2. Complete Phase 2: Foundational (shared headers)
3. Complete Phase 3: User Story 1 (cleanup)
4. **STOP and VALIDATE**: Build compiles, no duplicates remain
5. This alone delivers a navigable, trustworthy file tree

### Incremental Delivery

1. Setup + Foundational → shared headers exist
2. US1 (Cleanup) → clean file tree → verify build baseline
3. US2 (Defects) → zero build errors, clean EEPROM, fixed mixed-UART → verify `make all` clean
4. US3 (Build) → unified make targets → verify all 4 targets work
5. US7 (HW Wiring) → DIO pin config fixed, wiring reference card → verify bench setup (can start as early as step 2)
6. US4 (Research) → migration document → review with stakeholders (can start as early as step 2)
7. US5 (Docs) → accurate README → verify SC-003 (10-min onboarding) — requires steps 2–5 complete
8. US6 (Backlog) → enhancement roadmap → ready for next `/speckit.specify`
9. Polish → all success criteria verified → merge to main

### Single Developer Strategy

**Optimal sequential order**: Phase 1 → 2 → US1 → US2 → US3 → US7 → US4 → US5 → US6 → Polish.

US4 (research doc) can be drafted during any "waiting" period since it's documentation-only.
US7 can be started as early as after Phase 2 if blocking on US1/US2 review; it touches no shared files.

---

## Notes

- Total tasks: **79**
- Tasks per user story: US1=9, US2=16, US3=8, US7=12, US4=3, US5=7, US6=3, Setup=3, Foundational=8, Polish=10
- Parallel opportunities: 19 tasks marked [P], plus US7 fully parallel with US1–US3 track
- Minimal test tasks included per Constitution Principle V (T011, T035); full test suite deferred
- Header-comment tasks included per Constitution Principle VII (T010, T034, T055)
- Each story ends with a git commit for clean rollback points
- All file paths are relative to repository root (`/home/omar/iDMS/`)
- Deliverable docs (MCU report, enhancement backlog) placed under `docs/` at repository root
- US7 added in this revision: 12 new tasks covering DIO pin fixes (research.md §9), UART label corrections, wiring reference card with power rail verification (research.md §10), and connector pinouts (data-model.md §8)
- sendAlarmMessage() mixed-UART fix added to US2 (T031) — discovered during US7 pin cross-reference analysis
