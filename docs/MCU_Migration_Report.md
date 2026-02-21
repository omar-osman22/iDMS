# iDMS MCU Migration Research Report

**Status**: Research Complete — Migration Deferred  
**Date**: 2026-02-21  
**Source**: `specs/001-foundation-overhaul/research.md` §6  
**Decision**: Recommend **STM32F411CEU6** as primary migration target; **ESP32-S3** as secondary.

---

## Executive Summary

The iDMS firmware currently runs on an **ATmega128** (8-bit AVR, 16 MHz, 4 KB SRAM, 128 KB Flash). While functional, the ATmega128 is approaching its resource limits — the current firmware occupies ~34% of Flash and the RAM (.data + .bss) was exceeding the 4 KB limit during development.

Three platforms were evaluated as migration targets:

| | ATmega128 | STM32F411CEU6 | ESP32-S3 |
|--|-----------|---------------|----------|
| **Verdict** | Current (keep) | ✅ Primary recommendation | ✅ Secondary (if WiFi needed) |
| **Rationale** | Proven, but resource-constrained | Best analog + real-time fit | Built-in WiFi, noisier ADC |

**Recommendation**: Defer migration until the current cleanup is complete (this feature branch). The cleanup work makes the codebase migration-ready through clean MCAL abstractions. Migration should happen in a follow-up feature branch.

---

## Platform Comparison Matrix

| Criteria | ATmega128 (current) | STM32F411CEU6 | ESP32-S3-WROOM-1 |
|----------|---------------------|---------------|------------------|
| **CPU** | 8-bit AVR @ 16 MHz | 32-bit ARM Cortex-M4F @ 100 MHz | Dual-core Xtensa LX7 @ 240 MHz |
| **FPU** | No | Yes (single-precision) | Yes (single-precision) |
| **Flash** | 128 KB | 512 KB | 16 MB (external QSPI) |
| **SRAM** | 4 KB | 128 KB | 512 KB + 8 MB PSRAM (external) |
| **EEPROM** | 4 KB (native) | None (emulated in Flash) | None (NVS in Flash, ~16 KB) |
| **ADC** | 10-bit, 8 ch | 12-bit, 16 ch, 2.4 MSPS | 12-bit, 20 ch (~9 ENOB effective) |
| **UART** | 2 | 3 | 3 |
| **SPI** | 1 | 5 | 4 |
| **I2C** | 1 (TWI) | 3 | 2 |
| **WiFi** | No | No | 802.11 b/g/n |
| **Bluetooth** | No | No | BLE 5.0 |
| **USB** | No | USB OTG FS | USB OTG |
| **Watchdog** | Yes (HW) | Yes (IWDG + WWDG) | Yes (MWDT + RWDT) |
| **DMA** | No | Yes (16-stream) | Yes |
| **Power (active)** | ~15 mA | ~30 mA @ 100 MHz | ~80–240 mA |
| **Dev board cost** | ~$5 | ~$4–6 (BlackPill) | ~$6–10 (DevKitC) |
| **Chip cost (qty 1)** | ~$6 | ~$4 | ~$3 |
| **Toolchain** | avr-gcc | arm-none-eabi-gcc + STM32CubeMX | ESP-IDF (gcc-based) |
| **RTOS** | FreeRTOS (tight fit) | FreeRTOS / Zephyr / bare-metal | FreeRTOS (built-in) |
| **Industrial suitability** | Good (proven, low-power) | Excellent (industrial temp, CAN) | Good (WiFi advantage, noisier ADC) |

---

## iDMS Feature Mapping

| iDMS Feature | ATmega128 | STM32F411CEU6 | ESP32-S3 |
|-------------|-----------|---------------|----------|
| PT100 temperature (ADC) | PA0 (10-bit, 4.88 mV/LSB) | Any ADC pin (12-bit DMA, lower noise) | Any ADC pin (noisier, may need ext. ADC) |
| CT current (ADC) | PA1 (10-bit) | Any ADC pin (12-bit) | Any ADC pin |
| LCD — TopWay HMI (UART) | UART1 (PE0/PE1) @ 9600 | Any USART @ 9600 | Any UART @ 9600 |
| GSM SIM800L (UART) | UART0 (PD0/PD1) @ 9600 | Any USART @ 9600 | Any UART @ 9600 (or WiFi replaces GSM) |
| EEPROM (phone list, config, logs) | Native 4 KB | Flash emulation (HAL FLASH) or ext. I2C EEPROM | NVS Flash partition (built-in wear levelling) |
| GPIO — LEDs, buzzer, relay | PORTB, PORTC | Any GPIO (3.3 V logic — check relay driver) | Any GPIO (3.3 V) |
| Watchdog | HW WDT | IWDG (independent, cannot be disabled) | MWDT (can reset subsystems) |
| ISP programming | SPI (USBasp via J8) | SWD (faster, source-level debug capable) | USB/UART DFU bootloader |
| Debug interface | JTAG (limited) | SWD + ITM trace (excellent) | JTAG + OpenOCD |

---

## Migration Effort Breakdown

| Task | STM32F411 Effort | ESP32-S3 Effort | Notes |
|------|-----------------|-----------------|-------|
| MCAL/ADC rewrite | Small (~2 days) | Small (~2 days) | HAL/ESP-IDF ADC driver replaces register-level code |
| MCAL/UART rewrite | Small (~1 day) | Small (~1 day) | HAL USART / UART driver |
| MCAL/EEPROM → Flash/NVS | Medium (~3 days) | Small (~1 day) | STM32 needs wear-levelling; ESP NVS is ready |
| MCAL/DIO rewrite | Small (~1 day) | Small (~1 day) | GPIO HAL is straightforward |
| MCAL/Delay rewrite | Small (<1 day) | Small (<1 day) | `HAL_Delay` / `vTaskDelay` |
| Build: Makefile → CMake | Medium (~2 days) | Medium (~2 days) | STM32CubeIDE or CMake; ESP-IDF is CMake-native |
| Service layer changes | None | None | Abstractions are clean after this overhaul |
| Application layer changes | None | None | Logic is hardware-independent after overhaul |
| **Total estimated effort** | **~2–3 weeks** | **~2–3 weeks** | Teams familiar with ARM/Xtensa will be faster |

---

## Recommendation

### Primary: STM32F411CEU6 (WeAct BlackPill, ~$4–6)

**Recommended for iDMS next iteration.**

**Reasons:**

1. **Analog precision**: 12-bit ADC with hardware DMA offload reduces CPU overhead and produces higher-quality temperature/current measurements — critical for industrial accuracy.
2. **RAM headroom**: 128 KB SRAM (32× current) eliminates the RAM pressure that required disabling debug logging on ATmega128.
3. **Real-time determinism**: FPU and 100 MHz ARM Cortex-M4F = identical real-time behaviour with headroom for signal filtering (EMA, RMS) in floating-point.
4. **Debugging**: SWD + ITM trace provides step-through debugging with live variable watch — a major improvement over JTAG-only on ATmega128.
5. **Industrial range**: -40°C to +85°C temperature-range variants; CAN bus interface available for future expansion.
6. **Power**: ~30 mA active — suitable for battery-backed operation, no WiFi RF certification needed.
7. **Drop-in board size**: BlackPill (53 × 21 mm) is similar to Arduino Nano and fits existing enclosures.

### Secondary: ESP32-S3-WROOM-1 (Espressif DevKitC, ~$6–10)

**Recommended only if WiFi/cloud connectivity becomes a project requirement.**

**Reasons to consider:**
- Built-in WiFi/BLE could eliminate the SIM800L GSM module entirely for sites with WiFi coverage.
- NVS (Non-Volatile Storage) provides EEPROM replacement with built-in wear-levelling, simplifying the storage layer.
- Larger ecosystem for over-the-air (OTA) firmware updates.

**Concerns:**
- ADC noise (~9 ENOB effective vs 12-bit advertised) may require an external ADC (e.g., ADS1115 via I2C) for industrial measurement accuracy.
- Higher active power consumption (WiFi on: ~240 mA) limits battery-operated deployment.
- Larger binary size and longer build times with ESP-IDF.

### Migration Timing

**Defer migration** until the current foundation overhaul is complete. Rationale:

- The cleanup (US1–US3) produces a clean MCAL layer that is easy to port.
- All Service/ and Application/ code will be hardware-independent after US2.
- The unified Makefile (US3) has a clear `TARGET_HOST` / `TARGET_AVR` separation pattern that can be extended to `TARGET_STM32`.
- A follow-up feature branch (`002-stm32-migration`) is the appropriate vehicle.

---

## References

- `specs/001-foundation-overhaul/research.md` §6 — source data for this report
- `Hardware/Wiring_Reference.md` — ATmega128 pin assignments  
- `Config/eeprom_map.h` — current EEPROM layout (to be ported to Flash emulation or NVS)
- `MCAL/` — hardware abstraction layer (migration entry points)
