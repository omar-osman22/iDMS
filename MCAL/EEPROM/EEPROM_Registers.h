/*
 * EEPROM_Registers.h
 *
 *  Created on: Jun 7, 2023
 *      Author: Abdelrahman Hegazy
 */

#ifndef EEPROM_REGISTERS_H_
#define EEPROM_REGISTERS_H_

#define EEAR_REG			*((volatile u16*)0x3E)
#define EEDR_REG			*((volatile u8*)0x2D)
#define EECR_REG			*((volatile u8*)0x2C)
#define SREG_REG			*((volatile u8*)0x5F)


#endif /* EEPROM_REGISTERS_H_ */
