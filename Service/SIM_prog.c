/*
 * SIM_prog.c
 *
 *  Created on: Feb 14, 2022
 *      Author: pc
 */

#include	<avr/delay.h>
#include	<string.h>

#include 	"STD_TYPES.h"
#include 	"errorStates.h"
#include 	"SIM_interface.h"
#include 	"UART_Interface.h"
#include 	"DIO_interface.h"


u8 	SIM_u8Response[SIM_MAX_RESPONSE_LINES][SIM_MAX_RESPONSE_COLUMN] = {"0","0","0","0","0"};
u8 row = 0 , column = 0;

void SIM_voidSendCommand(u8 const *Copy_u8ATCommand)
{
	if(Copy_u8ATCommand != NULL)
	{
		UART_SendStringSync(Copy_u8ATCommand);
	}

}

void SIM_voidInit(void)
{
//  MUX_voidSwitch(GSM);
//  USART_disableRXInterrupt();
	// USART_enuInit();
}

ES_t SIM_estCheckIfReady(void)
{
	ES_t Local_enuerrorState = ES_NOK ;
	SIM_voidSendCommand("AT\r");
	//DIO_enuSetPinValue(DIO_PORTA,DIO_PIN1,DIO_HIGH);
	Local_enuerrorState = SIM_estGetResponse();

	return Local_enuerrorState;
}

ES_t SIM_estCheckIfRegistered(void)
{
	ES_t Local_enuerrorState = ES_NOK ;

	if(SIM_estCheckIfReady() == ES_OK)
	{

		//DIO_enuSetPinValue(DIO_PORTA,DIO_PIN1,DIO_HIGH);
		SIM_voidSendCommand("AT+CREG?\r");
		if(SIM_estGetResponse() == ES_OK)
		{

			if(strstr(SIM_u8Response[SIM_RESPONSE_LINE_2] , "+CREG: 0,1"))
			{
				Local_enuerrorState = ES_OK;
			}
		}
	}
	return Local_enuerrorState;
}

ES_t SIM_estGetResponse(void)
{
	for(row = SIM_RESPONSE_LINE_1 ; row < SIM_MAX_RESPONSE_LINES ; row++)
	{
		memset(SIM_u8Response[row],0,SIM_MAX_RESPONSE_COLUMN);
	}
	ES_t Local_enuerrorState = ES_NOK ;
	for(row = SIM_RESPONSE_LINE_1 ; row < SIM_MAX_RESPONSE_LINES ; row++)
	{
		for(column = SIM_RESPONSE_COLUMN_1 ; column < SIM_MAX_RESPONSE_COLUMN ; column++)
		{

			SIM_u8Response[row][column] = UART_ReceiveByteSynch();
			if(strstr(SIM_u8Response[row] , "\r\n"))
			{
				break;
			}
		}
		if(strstr(SIM_u8Response[row] , "OK\r\n"))
		{

			Local_enuerrorState = ES_OK;
			break;
		}
		else if(strstr(SIM_u8Response[row] , "ERROR\r\n"))
		{

			Local_enuerrorState = ES_ERROR;
			break;
		}

	}

	return Local_enuerrorState;
}

ES_t SIM_estSendSMS(u8 *Copy_u8PhoneNumber, u8 *Copy_u8SMS)
{
	ES_t Local_enuerrorState = ES_NOK ;
	if((Copy_u8PhoneNumber != NULL) && (Copy_u8SMS != NULL))
	{
		if(SIM_estCheckIfReady() == ES_OK)
		{
			SIM_voidSendCommand("AT+CMGF=1\r");
			_delay_ms(1000);
			if(SIM_estGetResponse() == ES_OK)
			{
				SIM_voidSendCommand("AT+CMGS=\"");
				SIM_voidSendCommand(Copy_u8PhoneNumber);
				SIM_voidSendCommand("\"\r");
				_delay_ms(1000);
				SIM_voidSendCommand(Copy_u8SMS);
				_delay_ms(100);
				SIM_voidSendCommand("\r");
				_delay_ms(1000);
				Local_enuerrorState = SIM_estGetResponse();
			}
		}
	}
	else
	{
		Local_enuerrorState = ES_NULL_POINTER ;
	}
	return Local_enuerrorState;
}

ES_t SIM_estSendLocation(u8 *Copy_u8PhoneNumber, u8 *Copy_u8Latitude, u8 *Copy_u8Longitude)
{
	ES_t Local_enuerrorState = ES_NOK ;
	if((Copy_u8PhoneNumber != NULL) && (Copy_u8Latitude != NULL) && (Copy_u8Longitude != NULL))
	{
		if(SIM_estCheckIfReady() == ES_OK)
		{
			SIM_voidSendCommand("AT+CMGF=1\r");
			if(SIM_estGetResponse() == ES_OK)
			{
				SIM_voidSendCommand("AT+CMGS=\"");
				SIM_voidSendCommand(Copy_u8PhoneNumber);
				SIM_voidSendCommand("\"\r");
				_delay_ms(10); //https://maps.go[dot]ogle.com/?q=
				//https://maps.google.com/?q=30.3052624,31.7730866&z=30
				SIM_voidSendCommand("Please delete \"[dot]\" form the link below then open it.\rhttps://maps.go[dot]ogle.com/?q="); //https://www.google.com/maps/dir/?api=1&parameters
				SIM_voidSendCommand(Copy_u8Latitude);
				SIM_voidSendCommand(",");
				SIM_voidSendCommand(Copy_u8Longitude);
				SIM_voidSendCommand("\r");
				Local_enuerrorState = SIM_estGetResponse();
			}
		}
	}
	else
	{
		Local_enuerrorState = ES_NULL_POINTER ;
	}
	return Local_enuerrorState;
}

ES_t SIM_estMakeCall(u8 *Copy_u8PhoneNumber)
{
	ES_t Local_enuerrorState = ES_NOK ;
	if(Copy_u8PhoneNumber != NULL)
	{
		if(SIM_estCheckIfReady() == ES_OK)
		{
			SIM_voidSendCommand("AT+CMGF=0\r");
			if(SIM_estGetResponse() == ES_OK)
			{
				SIM_voidSendCommand("ATD");
				SIM_voidSendCommand(Copy_u8PhoneNumber);
				SIM_voidSendCommand(";\r\n");
			}
		}

		Local_enuerrorState = SIM_estGetResponse();
	}
	else
	{
		Local_enuerrorState = ES_NULL_POINTER ;
	}
	return Local_enuerrorState;
}

void SIM_SendSMS(u8 const *Copy_u8PhoneNumber, u8 const *Copy_u8SMS)
{
	SIM_voidSendCommand("AT+CMGF=1\r");
	UART_SendByteSynch(10);
	_delay_ms(100);
	SIM_voidSendCommand("AT+CMGS=\"");
	UART_SendByteSynch(10);
	_delay_ms(100);
	SIM_voidSendCommand(Copy_u8PhoneNumber);
	UART_SendByteSynch(10);
	_delay_ms(100);
	SIM_voidSendCommand("\"\r");
	UART_SendByteSynch(10);
	_delay_ms(100);
	SIM_voidSendCommand(Copy_u8SMS);
	UART_SendByteSynch(10);
	_delay_ms(100);
	SIM_voidSendCommand("\r");
	UART_SendByteSynch(10);
	_delay_ms(100);

}

void SIM_MakeCall(u8 *Copy_u8PhoneNumber)
{
	SIM_voidSendCommand("ATD");
	//_delay_ms(10);
	SIM_voidSendCommand(Copy_u8PhoneNumber);
	//_delay_ms(10);
	SIM_voidSendCommand(";\r\n");
}
