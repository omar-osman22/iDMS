/*
 * TopWayLCD_Interface.h
 *
 *  Created on: Jun 7, 2023
 *      Author: Adel
 */

#ifndef TOPWAYLCD_INTERFACE_H_
#define TOPWAYLCD_INTERFACE_H_

#define In          	0x00080000
#define Out    	        0x00080002
#define In_Num    	    0x0008000E
#define Out_Num     	0x00080010
#define Validation   	0x00000100
#define Add          	0x0008000A
#define Remove       	0x00080012
#define L_type       	0x00080008
#define Phone        	0x00000080
#define CList_address 	0x00000180
#define SList_address   0x00000F00
#define Clear_BUFF      0x00080004
#define Calling_list    1
#define SMS_list        2

void LCD_Init();

void LCD_SendNum16(u32 Copy_u32Address , u16 Copy_u16Num);

u16 LCD_GetNum16(u32 Copy_u32Address);

void LCD_SendString(u32 Copy_u32Address , u8 *Copy_u16Str );

void LCD_GetString(u32 Copy_u32Address , u8 *Ptr_ReturnStr);

#endif /* TOPWAYLCD_INTERFACE_H_ */
