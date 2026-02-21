/*
 * delay.h — MCAL Delay Abstraction
 *
 * Feature: 001-foundation-overhaul
 * Date: 2026-02-21
 *
 * PURPOSE:
 *   Portable delay abstraction. Service and Application code
 *   MUST use MCAL_DelayMs() instead of including avr/delay.h
 *   or using bare for-loops.
 *
 * LAYER: MCAL
 * DEPENDS ON: Platform-specific delay implementation
 * USED BY: Service/SIM_prog, Service/TopWayLCD_Program, main.c
 */

#ifndef DELAY_H_
#define DELAY_H_

#ifdef TARGET_AVR
  #ifndef F_CPU
    #define F_CPU 16000000UL
  #endif
  #include <util/delay.h>
  #define MCAL_DelayMs(ms)   _delay_ms(ms)
  #define MCAL_DelayUs(us)   _delay_us(us)
#elif defined(TARGET_HOST)
  /* No-op delays in test/host builds */
  #define MCAL_DelayMs(ms)   ((void)0)
  #define MCAL_DelayUs(us)   ((void)0)
#else
  #error "Define TARGET_AVR or TARGET_HOST to select delay implementation"
#endif

#endif /* DELAY_H_ */
