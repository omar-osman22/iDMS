/*
 * INTERNAL_EEPROM.h
 *
 *  Created on: Jun 7, 2023
 *      Author: Abdelrahman Hegazy
 *
 *  Updated: 2026-02-21 (Feature: 001-foundation-overhaul)
 *  PURPOSE: Unified EEPROM Abstraction API
 *  DEPENDS ON: EEPROM_Registers.h
 *  USED BY: Service/PhoneList, Service/data_logger, Service/error_handler, Config/system_config
 */

#ifndef INTERNAL_EEPROM_H_
#define INTERNAL_EEPROM_H_

#include "../../Utilities/STD_TYPES.h"

void EEPROM_INIT();

void EEPROM_WriteByte(u16 Location,u8 Data);

u8 	 EEPROM_ReadByte(u16 location);

/* ── Legacy name aliases (backward compatibility for existing callers) ── */
#define EEPROM_voidWriteData(addr, data)  EEPROM_WriteByte((addr), (data))
#define EEPROM_u8ReadData(addr)           EEPROM_ReadByte((addr))

/* ── Block read/write functions (to replace avr/eeprom.h block ops) ── */
void EEPROM_WriteBlock(u16 address, const u8 *data, u16 length);
void EEPROM_ReadBlock(u16 address, u8 *data, u16 length);

#endif /* INTERNAL_EEPROM_H_ */
