/*
 * UART_Interface.h
 *
 *  Created on: May 27, 2023
 *      Author: aaa
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

void UART_Init(void);

void UART_SendByteSynch(u8 Data);

void UART_SendByteASynch(u8 * Data);

u8 UART_ReceiveByteSynch();

void UART_ReceiveByteASynch(u8 * Data);

void UART_SendStringSync(u8 const* Copy_pu8Data );


void UART_voidInit();

void UART_voidWriteData(u8 Copy_u8Data);

u8 UART_u8ReadData();

void UART_voidWriteString(u8 *Copy_u8Data);

void UART_voidReadString(u8 *Ptr_ReturnStr);
void UART_voidDisable();
#endif /* UART_INTERFACE_H_ */
