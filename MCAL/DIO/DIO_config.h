/*
 * DIO_config.h — Digital I/O Pin Direction Configuration
 *
 * Feature: 001-foundation-overhaul (US7)
 * Date: 2026-02-21
 *
 * PURPOSE:
 *   Corrected pin direction configuration for all ATmega128 I/O ports (A–G).
 *   Cross-referenced against Hardware/Hardware_Integration.md pin assignments
 *   and Hardware/Wiring_Reference.md.
 *
 * SOURCE: Derived from contracts/dio_config_corrected.h
 * LAYER: MCAL (DIO configuration)
 * DEPENDS ON: DIO_private.h (DIO_INPUT, DIO_OUTPUT definitions)
 * USED BY: MCAL/DIO/DIO_program.c (MDIO_vInt initializer)
 *
 * UART PERIPHERAL ASSIGNMENT (actual firmware code — differs from some HW docs):
 *   - UART0 (PD0/PD1) → GSM module (SIM800L), Connector J5
 *   - UART1 (PE0/PE1) → LCD display (TopWay HMI), Connector J4
 *
 * CHANGE HISTORY:
 *   2026-02-21  Foundation overhaul: corrected Port A (all INPUT/ADC),
 *               Port B (PB0/PB3 INPUT for ISP), Port C (PC0/PC2/PC3 OUTPUT),
 *               Port D (PD1 OUTPUT for UART0 TX), added Port E/F/G.
 */

#ifndef DIO_CONFIG_H_
#define DIO_CONFIG_H_

/* ── PORT A: ADC Inputs (analog sensor conditioning) ── */
#define DIO_PIN_A0_DIR        DIO_INPUT   /* PA0: PT100 temperature (ADC0) */
#define DIO_PIN_A1_DIR        DIO_INPUT   /* PA1: CT current sensor (ADC1) */
#define DIO_PIN_A2_DIR        DIO_INPUT   /* PA2: Reserved ADC */
#define DIO_PIN_A3_DIR        DIO_INPUT   /* PA3: Reserved ADC */
#define DIO_PIN_A4_DIR        DIO_INPUT   /* PA4: Battery voltage monitor */
#define DIO_PIN_A5_DIR        DIO_INPUT   /* PA5: Supply voltage monitor */
#define DIO_PIN_A6_DIR        DIO_INPUT   /* PA6: Reserved */
#define DIO_PIN_A7_DIR        DIO_INPUT   /* PA7: Reserved */

/* ── PORT B: ISP, LEDs, Buzzer ── */
#define DIO_PIN_B0_DIR        DIO_INPUT   /* PB0: External timer input (T0) */
#define DIO_PIN_B1_DIR        DIO_OUTPUT  /* PB1: ISP SCK */
#define DIO_PIN_B2_DIR        DIO_OUTPUT  /* PB2: ISP MOSI */
#define DIO_PIN_B3_DIR        DIO_INPUT   /* PB3: ISP MISO */
#define DIO_PIN_B4_DIR        DIO_OUTPUT  /* PB4: PWM output (cooling fan) */
#define DIO_PIN_B5_DIR        DIO_OUTPUT  /* PB5: Status LED 1 */
#define DIO_PIN_B6_DIR        DIO_OUTPUT  /* PB6: Status LED 2 */
#define DIO_PIN_B7_DIR        DIO_OUTPUT  /* PB7: Alarm buzzer */

/* ── PORT C: GSM Control, Relays, Inputs ── */
#define DIO_PIN_C0_DIR        DIO_OUTPUT  /* PC0: GSM module RESET control */
#define DIO_PIN_C1_DIR        DIO_INPUT   /* PC1: GSM module STATUS input */
#define DIO_PIN_C2_DIR        DIO_OUTPUT  /* PC2: Relay 1 — alarm output */
#define DIO_PIN_C3_DIR        DIO_OUTPUT  /* PC3: Relay 2 — fault output */
#define DIO_PIN_C4_DIR        DIO_INPUT   /* PC4: External interrupt input */
#define DIO_PIN_C5_DIR        DIO_INPUT   /* PC5: System enable/disable */
#define DIO_PIN_C6_DIR        DIO_INPUT   /* PC6: Reserved */
#define DIO_PIN_C7_DIR        DIO_INPUT   /* PC7: Emergency stop (active low) */

/* ── PORT D: UART0 (GSM via SIM800L), External Interrupts ── */
#define DIO_PIN_D0_DIR        DIO_INPUT   /* PD0: UART0 RX — GSM receive */
#define DIO_PIN_D1_DIR        DIO_OUTPUT  /* PD1: UART0 TX — GSM transmit */
#define DIO_PIN_D2_DIR        DIO_INPUT   /* PD2: INT0 — emergency stop */
#define DIO_PIN_D3_DIR        DIO_INPUT   /* PD3: INT1 — user input */
#define DIO_PIN_D4_DIR        DIO_INPUT   /* PD4: ICP1 — input capture */
#define DIO_PIN_D5_DIR        DIO_INPUT   /* PD5: XCK0 — external clock */
#define DIO_PIN_D6_DIR        DIO_INPUT   /* PD6: Reserved */
#define DIO_PIN_D7_DIR        DIO_INPUT   /* PD7: Reserved */

/* ── PORT E: UART1 (LCD via TopWay), SPI Chip Selects ── */
#define DIO_PIN_E0_DIR        DIO_INPUT   /* PE0: UART1 RX — LCD receive */
#define DIO_PIN_E1_DIR        DIO_OUTPUT  /* PE1: UART1 TX — LCD transmit */
#define DIO_PIN_E2_DIR        DIO_INPUT   /* PE2: XCK1 — external clock */
#define DIO_PIN_E3_DIR        DIO_OUTPUT  /* PE3: SD card CS (future) */
#define DIO_PIN_E4_DIR        DIO_OUTPUT  /* PE4: SPI chip select 1 */
#define DIO_PIN_E5_DIR        DIO_OUTPUT  /* PE5: SPI chip select 2 */
#define DIO_PIN_E6_DIR        DIO_INPUT   /* PE6: Reserved */
#define DIO_PIN_E7_DIR        DIO_INPUT   /* PE7: Reserved */

/* ── PORT F: ADC Alternate Channels / JTAG ── */
#define DIO_PIN_F0_DIR        DIO_INPUT   /* PF0: Alternate ADC0 */
#define DIO_PIN_F1_DIR        DIO_INPUT   /* PF1: Alternate ADC1 */
#define DIO_PIN_F2_DIR        DIO_INPUT   /* PF2: Alternate ADC2 */
#define DIO_PIN_F3_DIR        DIO_INPUT   /* PF3: Alternate ADC3 */
#define DIO_PIN_F4_DIR        DIO_INPUT   /* PF4: JTAG TCK */
#define DIO_PIN_F5_DIR        DIO_INPUT   /* PF5: JTAG TMS */
#define DIO_PIN_F6_DIR        DIO_OUTPUT  /* PF6: JTAG TDO */
#define DIO_PIN_F7_DIR        DIO_INPUT   /* PF7: JTAG TDI */

/* ── PORT G: Crystal, Watchdog, User I/O ── */
#define DIO_PIN_G0_DIR        DIO_OUTPUT  /* PG0: Crystal driver output */
#define DIO_PIN_G1_DIR        DIO_INPUT   /* PG1: Crystal driver input */
#define DIO_PIN_G2_DIR        DIO_OUTPUT  /* PG2: Watchdog reset output */
#define DIO_PIN_G3_DIR        DIO_INPUT   /* PG3: User button input */
#define DIO_PIN_G4_DIR        DIO_OUTPUT  /* PG4: Status indicator output */

#endif /* DIO_CONFIG_H_ */
