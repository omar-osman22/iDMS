/*
 * INTERNAL_EEPROM.c
 *
 *  Created on: Jun 7, 2023
 *      Author: Abdelrahman Hegazy
 */

#include "../Utilities/BIT_MATH.h"
#include "../Utilities/STD_TYPES.h"
#include "../MCAL/EEPROM/INTERNAL_EEPROM.h"
#include "../MCAL/EEPROM/EEPROM_Registers.h"


void EEPROM_INIT()
{
	SET_BIT(EECR_REG,2);
	SET_BIT(EECR_REG,1);
}

void EEPROM_WriteByte(u16 Location,u8 Data)
{
	CLR_BIT(SREG_REG,7);

	EEDR_REG = Data;

	EEAR_REG = Location;

	SET_BIT(EECR_REG,2);

	SET_BIT(EECR_REG,1);

	SET_BIT(SREG_REG,7);

	while(GET_BIT(EECR_REG,1) == 1);
}


u8 	 EEPROM_ReadByte(u16 location)
{

	CLR_BIT(SREG_REG,7);

	EEAR_REG = location;

	SET_BIT(EECR_REG,0);

	SET_BIT(SREG_REG,7);

	return EEDR_REG;
}
