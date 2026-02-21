/*
 * sensor_common.h — Shared Sensor Constants Contract
 *
 * Feature: 001-foundation-overhaul
 * Date: 2026-02-14
 *
 * PURPOSE:
 *   Single definition point for constants shared across
 *   sensor configuration files. Eliminates the duplicate
 *   #define VREF in PT100_config.h and CTcurrent_config.h.
 *
 * LAYER: Config (available to all layers)
 * DEPENDS ON: None
 * USED BY: Service/PT100_config.h, Service/CTcurrent_config.h,
 *          and any future sensor configs
 *
 * NOTE: This is a CONTRACT file. The implementation will be placed
 *       at Config/sensor_common.h.
 */

#ifndef SENSOR_COMMON_CONTRACT_H
#define SENSOR_COMMON_CONTRACT_H

/* Reference voltage for ADC in millivolts */
#define VREF                4845u

/* ADC resolution (10-bit on ATmega128) */
#define ADC_RESOLUTION      1024u

/* Source voltage for power calculation (V) */
#define SOURCE_VOLTAGE      220u

/* System tick interval in milliseconds */
#define SYSTEM_TICK_MS      10u

#endif /* SENSOR_COMMON_CONTRACT_H */
