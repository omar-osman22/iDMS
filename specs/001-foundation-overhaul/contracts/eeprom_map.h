/*
 * eeprom_map.h — EEPROM Memory Map Contract
 *
 * Feature: 001-foundation-overhaul
 * Date: 2026-02-14
 *
 * PURPOSE:
 *   Single source of truth for all EEPROM region addresses and sizes.
 *   Every module that reads/writes EEPROM MUST include this header
 *   and use these macros — never hard-coded addresses.
 *
 * LAYER: Config (available to all layers)
 * DEPENDS ON: None
 * USED BY: All EEPROM-accessing modules
 *
 * NOTE: This is a CONTRACT file. The implementation file will be
 *       placed at Config/eeprom_map.h in the actual codebase.
 */

#ifndef EEPROM_MAP_CONTRACT_H
#define EEPROM_MAP_CONTRACT_H

/* ── Total EEPROM size (ATmega128) ── */
#define EEPROM_TOTAL_SIZE       4096u   /* 0x000 – 0xFFF */

/* ── Region: Phone List ── */
#define EEPROM_PHONE_LIST_BASE  0x000u
#define EEPROM_PHONE_LIST_SIZE  0x200u  /* 512 bytes */
#define EEPROM_PHONE_LIST_END   (EEPROM_PHONE_LIST_BASE + EEPROM_PHONE_LIST_SIZE - 1u)

/* ── Region: System Configuration ── */
#define EEPROM_CONFIG_BASE      0x200u
#define EEPROM_CONFIG_SIZE      0x080u  /* 128 bytes */
#define EEPROM_CONFIG_END       (EEPROM_CONFIG_BASE + EEPROM_CONFIG_SIZE - 1u)

/* ── Region: Data Logger ── */
#define EEPROM_DATALOG_BASE     0x280u
#define EEPROM_DATALOG_SIZE     0x400u  /* 1024 bytes (40 entries × 25 B + header) */
#define EEPROM_DATALOG_END      (EEPROM_DATALOG_BASE + EEPROM_DATALOG_SIZE - 1u)
#define EEPROM_DATALOG_MAX_ENTRIES  40u

/* ── Region: Error Log ── */
#define EEPROM_ERRORLOG_BASE    0x680u
#define EEPROM_ERRORLOG_SIZE    0x180u  /* 384 bytes (20 entries × 19 B + header) */
#define EEPROM_ERRORLOG_END     (EEPROM_ERRORLOG_BASE + EEPROM_ERRORLOG_SIZE - 1u)
#define EEPROM_ERRORLOG_MAX_ENTRIES  20u

/* ── Region: Reserved (future use) ── */
#define EEPROM_RESERVED_BASE    0x800u
#define EEPROM_RESERVED_SIZE    0x800u  /* 2048 bytes */
#define EEPROM_RESERVED_END     (EEPROM_RESERVED_BASE + EEPROM_RESERVED_SIZE - 1u)

/* ── Compile-time overlap guards ── */
/* These assertions fire at compile time if any regions overlap */

_Static_assert(
    EEPROM_PHONE_LIST_BASE + EEPROM_PHONE_LIST_SIZE <= EEPROM_CONFIG_BASE,
    "EEPROM overlap: Phone List overflows into System Config region"
);

_Static_assert(
    EEPROM_CONFIG_BASE + EEPROM_CONFIG_SIZE <= EEPROM_DATALOG_BASE,
    "EEPROM overlap: System Config overflows into Data Log region"
);

_Static_assert(
    EEPROM_DATALOG_BASE + EEPROM_DATALOG_SIZE <= EEPROM_ERRORLOG_BASE,
    "EEPROM overlap: Data Log overflows into Error Log region"
);

_Static_assert(
    EEPROM_ERRORLOG_BASE + EEPROM_ERRORLOG_SIZE <= EEPROM_RESERVED_BASE,
    "EEPROM overlap: Error Log overflows into Reserved region"
);

_Static_assert(
    EEPROM_RESERVED_BASE + EEPROM_RESERVED_SIZE <= EEPROM_TOTAL_SIZE,
    "EEPROM overflow: Reserved region exceeds total EEPROM size"
);


#endif /* EEPROM_MAP_CONTRACT_H */
