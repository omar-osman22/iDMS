/*
 * TopWayLCD_Program.c
 *
 *  Created on: Jun 7, 2023
 *      Author: Adel
 */

#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include "../MCAL/UART/UART_Interface.h"
#include "../Service/TopWayLCD_Interface.h"
#include "avr/delay.h"


void LCD_Init()
{
	UART_voidInit();
}

void LCD_SendNum16(u32 Copy_u32Address , u16 Copy_u16Num)
{
	u8 add_B1 = (u8)Copy_u32Address;
	u8 add_B2 = (u8)(Copy_u32Address >> 8);
	u8 add_B3 = (u8)(Copy_u32Address >> 16);
	u8 add_B4 = (u8)(Copy_u32Address >> 24);

	u8 data_B1 = (u8)Copy_u16Num;
	u8 data_B2 = (u8)(Copy_u16Num >> 8);

	 UART_voidWriteData(0xaa);
	 UART_voidWriteData(0x3d);
	 UART_voidWriteData(add_B4);
	 UART_voidWriteData(add_B3);
	 UART_voidWriteData(add_B2);
	 UART_voidWriteData(add_B1);
	 UART_voidWriteData(data_B2);
	 UART_voidWriteData(data_B1);
	 UART_voidWriteData(0xcc);
	 UART_voidWriteData(0x33);
	 UART_voidWriteData(0xc3);
	 UART_voidWriteData(0x3c);
}

u16 LCD_GetNum16(u32 Copy_u32Address)
{
	u8 add_B1 = (u8)Copy_u32Address;
	u8 add_B2 = (u8)(Copy_u32Address >> 8);
	u8 add_B3 = (u8)(Copy_u32Address >> 16);
	u8 add_B4 = (u8)(Copy_u32Address >> 24);

	u16 data = 0;

	UART_voidWriteData(0xaa);
	UART_voidWriteData(0x3e);
	UART_voidWriteData(add_B4);
	UART_voidWriteData(add_B3);
	UART_voidWriteData(add_B2);
	UART_voidWriteData(add_B1);
	UART_voidWriteData(0xcc);
	UART_voidWriteData(0x33);
	UART_voidWriteData(0xc3);
	UART_voidWriteData(0x3c);

	while(UART_u8ReadData() != 0xaa){}
	while(UART_u8ReadData() != 0x3e){}

	data = UART_u8ReadData();
	data = data << 8;
	data = UART_u8ReadData();

	while(UART_u8ReadData()!=0xcc){}
	while(UART_u8ReadData()!=0x33){}
	while(UART_u8ReadData()!=0xc3){}
	while(UART_u8ReadData()!=0x3c){}

	return data;
}

void LCD_SendString(u32 Copy_u32Address , u8 *Copy_u16Str )
{
	u8 add_B1 = (u8)Copy_u32Address;
	u8 add_B2 = (u8)(Copy_u32Address >> 8);
	u8 add_B3 = (u8)(Copy_u32Address >> 16);
	u8 add_B4 = (u8)(Copy_u32Address >> 24);

	 UART_voidWriteData(0xaa);
	 UART_voidWriteData(0x42);
	 UART_voidWriteData(add_B4);
	 UART_voidWriteData(add_B3);
	 UART_voidWriteData(add_B2);
	 UART_voidWriteData(add_B1);
	 UART_voidWriteString(Copy_u16Str);
	 UART_voidWriteData(0xcc);
	 UART_voidWriteData(0x33);
	 UART_voidWriteData(0xc3);
	 UART_voidWriteData(0x3c);
}

void LCD_GetString(u32 Copy_u32Address , u8 *Ptr_ReturnStr)
{
	u8 add_B1 = (u8)Copy_u32Address;
	u8 add_B2 = (u8)(Copy_u32Address >> 8);
	u8 add_B3 = (u8)(Copy_u32Address >> 16);
	u8 add_B4 = (u8)(Copy_u32Address >> 24);

	UART_voidWriteData(0xaa);
	UART_voidWriteData(0x43);
	UART_voidWriteData(add_B4);
	UART_voidWriteData(add_B3);
	UART_voidWriteData(add_B2);
	UART_voidWriteData(add_B1);
	UART_voidWriteData(0xcc);
	UART_voidWriteData(0x33);
	UART_voidWriteData(0xc3);
	UART_voidWriteData(0x3c);

	while(UART_u8ReadData() != 0xaa){}
	while(UART_u8ReadData() != 0x43){}

	UART_voidReadString(Ptr_ReturnStr);

	while(UART_u8ReadData()!=0xcc){}
	while(UART_u8ReadData()!=0x33){}
	while(UART_u8ReadData()!=0xc3){}
	while(UART_u8ReadData()!=0x3c){}
}
