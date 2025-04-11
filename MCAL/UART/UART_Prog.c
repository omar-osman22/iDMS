/*
 * UART_Prog.c
 *
 *  Created on: May 27, 2023
 *      Author: aaa
 */

#include "../Utilities/BIT_MATH.h"
#include "../Utilities/STD_TYPES.h"
#include "../MCAL/UART/UART_Registers.h"
#include "../MCAL/UART/UART_Interface.h"


UCSRC0_REG_Bits UCSRC0;
UCSRB0_REG_Bits UCSRB0;

u8 * GlobalPtr_SendDataAsynch = NULL;

u8 * GlobalPtr_RecDataAsynch = NULL;

void UART_Init(void)
{
	//select character size (8bit)
	SET_BIT(UCSRC0_REG,1);
	SET_BIT(UCSRC0_REG,2);
	//Set Baud Rate to 9600
	UBRRL0_REG = 103;

	//enable for Tx Circuit
	SET_BIT(UCSRB0_REG,3);
	//enable for Rx Circuit
	SET_BIT(UCSRB0_REG,4);
}

void UART_SendByteSynch(u8 Data)
{
	//wait untile UDR is Empty
	while(GET_BIT(UCSRA0_REG,5) == 0);
	//put data in UDR
	UDR0_REG = Data;
	//wait until transmitting is complete
	while(GET_BIT(UCSRA0_REG,6) == 0);
}

void UART_SendByteASynch(u8 * Data)
{
	GlobalPtr_SendDataAsynch = Data;
}

u8 UART_ReceiveByteSynch()
{
	//wait until receive is complete
	while(GET_BIT(UCSRA0_REG,7) == 0);

	//return Data
	return UDR0_REG;
}

void UART_ReceiveByteASynch(u8 * Data)
{
	Data = GlobalPtr_RecDataAsynch;
}

void UART_SendStringSync(u8 const* Copy_pu8Data )
{
	while( *Copy_pu8Data != '\0')
	{
		while(GET_BIT(UCSRA0_REG,5) == 0);

		UDR0_REG = *Copy_pu8Data ;

		while(GET_BIT(UCSRA0_REG,6) == 0);

		Copy_pu8Data++ ;
	}
}

void UART_voidInit()
{
	/*Select character size it is 2 in mode*/
	CLR_BIT(UCSRB , UCSRB_UCSZ2);
	/*Select UCSRC register - Asynchronous mode -no parity - 1 stop bit - 8 bit character - no polarity */
	UCSRC = 0b00000110;
	/*Set the baudRate 9600 bps (modified from 115.2K)*/
	UBBRL = 103; /* Changed from 8 to 103 to match 9600 baud rate */
	/*Enable the RX and TX */
	SET_BIT(UCSRB , UCSRB_RXEN);
	SET_BIT(UCSRB , UCSRB_TXEN);
}
void UART_voidWriteData(u8 Copy_u8Data)
{
	/*wait until the transmit buffer empty and ready*/
	while(GET_BIT(UCSRA,UCSRA_UDRE) == 0);
	UDR = Copy_u8Data;
}
u8 UART_u8ReadData()
{
	u8 data;

	while(GET_BIT(UCSRA,UCSRA_RXC) ==0);

	return UDR;
}

void UART_voidWriteString(u8 *Copy_u8Data)
{
	while( *Copy_u8Data != '\0')
	{
		while(GET_BIT(UCSRA,UCSRA_UDRE) == 0);

		UDR = *Copy_u8Data;

		Copy_u8Data++;
	}
	while(GET_BIT(UCSRA,UCSRA_UDRE) == 0);

	UDR = 0x00;

}

void UART_voidReadString(u8 *Ptr_ReturnStr)
{
	while(GET_BIT(UCSRA,UCSRA_RXC) ==0);
	*Ptr_ReturnStr = UDR;

	while(*Ptr_ReturnStr != '\0')
	{
		while(GET_BIT(UCSRA,UCSRA_RXC) ==0);
		*(++Ptr_ReturnStr) = UDR;
	}
}

void UART_voidDisable()
{
	CLR_BIT(UCSRB,4);
	CLR_BIT(UCSRB,3);
}
