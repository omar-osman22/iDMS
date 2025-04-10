/*
 * UART_Registers.h
 *
 *  Created on: May 27, 2023
 *      Author: aaa
 */

#ifndef UART_REGISTERS_H_
#define UART_REGISTERS_H_


#define UDR0_REG			*((volatile u8*)0x2C)
#define UCSRA0_REG			*((volatile u8*)0x2B)
#define UCSRB0_REG			*((volatile u8*)0x2A)
#define UCSRC0_REG			*((volatile u8*)0x95)
#define UBRRL0_REG			*((volatile u8*)0x29)


typedef union
{
	struct
	{
		u8 ClockPolarityBit:1;
		u8 CharSizeBits:2;
		u8 StopBit:1;
		u8 ParityBits:2;
		u8 UARTModeBit:1;
		u8 ReservedBit:1;
	}Bits;
	u8 Byte;
}UCSRC0_REG_Bits;

typedef union
{
	struct
	{
		u8 ReservedBits:3;
		u8 TransmitterEnableBit:1;
		u8 ReceiverEnableBit:1;
		u8 RegEmpIntEnBit:1;
		u8 TranCompIntEnBit:1;
		u8 RecCompIntEnBit:1;
	}Bits;
	u8 Byte;
}UCSRB0_REG_Bits;

#define UDR				*((volatile u8*)0x9C)


#define UCSRA             *((volatile u8*)0x9B)
#define UCSRA_RXC         7
#define UCSRA_TXC         6
#define UCSRA_UDRE        5

#define UCSRB			  *((volatile u8*)0x9A)
#define UCSRB_RXEN          4
#define UCSRB_TXEN          3
#define UCSRB_UCSZ2       	2

#define UCSRC			  *((volatile u8*)0x9D)
#define UCSRC_UMSEL         6
#define UCSRC_UPM1          5
#define UCSRC_UPM0          4
#define UCSRC_USBS          3
#define UCSRC_UCSZ1         2
#define UCSRC_UCSZ0         1
#define UCSRC_UCPOL         0

#define UBBRL			  *((volatile u8*)0x99)

#endif /* UART_REGISTERS_H_ */
