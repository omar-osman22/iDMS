/*
 * eeprom_api.h — Unified EEPROM Abstraction API Contract
 *
 * Feature: 001-foundation-overhaul
 * Date: 2026-02-14
 *
 * PURPOSE:
 *   Defines the canonical EEPROM read/write API that ALL modules
 *   must use. This contract replaces the three incompatible APIs
 *   that currently coexist in the codebase:
 *     1. EEPROM_WriteByte / EEPROM_ReadByte (MCAL implementation)
 *     2. EEPROM_voidWriteData / EEPROM_u8ReadData (Service callers)
 *     3. avr/eeprom.h direct calls (PhoneList.c)
 *
 * LAYER: MCAL (Microcontroller Abstraction Layer)
 * DEPENDS ON: EEPROM_Registers.h (register definitions)
 * USED BY: Service/PhoneList, Service/data_logger, Service/error_handler,
 *          Config/system_config
 *
 * NOTE: This is a CONTRACT file — it defines the target API signatures.
 *       The actual implementation lives in MCAL/EEPROM/INTERNAL_EEPROM.c.
 *       This file should be used as the reference when implementing the
 *       unified header.
 */

#ifndef EEPROM_API_CONTRACT_H
#define EEPROM_API_CONTRACT_H

/* ── Core read/write functions (implemented in INTERNAL_EEPROM.c) ── */

/**
 * Write a single byte to EEPROM.
 *
 * @param address  EEPROM address (0x000–0xFFF for ATmega128)
 * @param data     Byte value to write
 *
 * Preconditions:
 *   - address < EEPROM_TOTAL_SIZE (4096)
 *   - Interrupts may be temporarily disabled during write
 *
 * Postconditions:
 *   - EEPROM byte at `address` contains `data`
 *   - Function blocks until write is complete (~8.5 ms on ATmega128)
 */
void EEPROM_WriteByte(unsigned short address, unsigned char data);

/**
 * Read a single byte from EEPROM.
 *
 * @param address  EEPROM address (0x000–0xFFF for ATmega128)
 * @return         Byte value at the given address
 *
 * Preconditions:
 *   - address < EEPROM_TOTAL_SIZE (4096)
 *
 * Postconditions:
 *   - Returns the byte stored at `address`
 *   - Non-blocking (EEPROM reads are fast)
 */
unsigned char EEPROM_ReadByte(unsigned short address);


/* ── Legacy name aliases (backward compatibility for existing callers) ── */

/**
 * These macros provide backward compatibility for Service-layer callers
 * that were written using the Hungarian naming convention. New code
 * SHOULD use EEPROM_WriteByte / EEPROM_ReadByte directly.
 */
#define EEPROM_voidWriteData(addr, data)  EEPROM_WriteByte((addr), (data))
#define EEPROM_u8ReadData(addr)           EEPROM_ReadByte((addr))


/* ── Block read/write functions (to replace avr/eeprom.h block ops) ── */

/**
 * Write a block of bytes to EEPROM.
 *
 * @param address  Starting EEPROM address
 * @param data     Pointer to source data buffer
 * @param length   Number of bytes to write
 *
 * Preconditions:
 *   - address + length <= EEPROM_TOTAL_SIZE
 *   - data is not NULL
 *   - length > 0
 *
 * Postconditions:
 *   - EEPROM bytes [address, address+length) contain data[0..length)
 *   - Blocks until all writes complete
 *
 * Note: This function calls EEPROM_WriteByte in a loop. It does NOT
 *       implement wear-leveling; callers should check for value changes
 *       before writing.
 */
void EEPROM_WriteBlock(unsigned short address,
                       const unsigned char *data,
                       unsigned short length);

/**
 * Read a block of bytes from EEPROM.
 *
 * @param address  Starting EEPROM address
 * @param buffer   Pointer to destination buffer
 * @param length   Number of bytes to read
 *
 * Preconditions:
 *   - address + length <= EEPROM_TOTAL_SIZE
 *   - buffer is not NULL
 *   - length > 0
 *
 * Postconditions:
 *   - buffer[0..length) contains EEPROM data from [address, address+length)
 */
void EEPROM_ReadBlock(unsigned short address,
                      unsigned char *buffer,
                      unsigned short length);


#endif /* EEPROM_API_CONTRACT_H */
