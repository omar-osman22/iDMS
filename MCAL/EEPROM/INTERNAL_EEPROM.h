/*
 * INTERNAL_EEPROM.h
 *
 *  Created on: Jun 7, 2023
 *      Author: Abdelrahman Hegazy
 */

#ifndef INTERNAL_EEPROM_H_
#define INTERNAL_EEPROM_H_

void EEPROM_INIT();

void EEPROM_WriteByte(u16 Location,u8 Data);

u8 	 EEPROM_ReadByte(u16 location);

#endif /* INTERNAL_EEPROM_H_ */
