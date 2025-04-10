/*****************************************************************/
/* Author  : Ahmed Samy                                               */
/* SWC     : DIO                                                 */
/* Version : 1.0                                                 */
/* Date    : 07 Dec 2023                                         */
/*****************************************************************/
#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include "DIO_interface.h"
#include "DIO_config.h"
#include "DIO_private.h"

/*
void MDIO_vInt (void)
{
	DDRA = CONCAT(DIO_PIN_A7_DIR,DIO_PIN_A6_DIR,DIO_PIN_A5_DIR,DIO_PIN_A4_DIR,DIO_PIN_A3_DIR,DIO_PIN_A2_DIR,DIO_PIN_A1_DIR,DIO_PIN_A0_DIR);
	DDRB = CONCAT(DIO_PIN_B7_DIR,DIO_PIN_B6_DIR,DIO_PIN_B5_DIR,DIO_PIN_B4_DIR,DIO_PIN_B3_DIR,DIO_PIN_B2_DIR,DIO_PIN_B1_DIR,DIO_PIN_B0_DIR);
	DDRC = CONCAT(DIO_PIN_C7_DIR,DIO_PIN_C6_DIR,DIO_PIN_C5_DIR,DIO_PIN_C4_DIR,DIO_PIN_C3_DIR,DIO_PIN_C2_DIR,DIO_PIN_C1_DIR,DIO_PIN_C0_DIR);
	DDRD = CONCAT(DIO_PIN_D7_DIR,DIO_PIN_D6_DIR,DIO_PIN_D5_DIR,DIO_PIN_D4_DIR,DIO_PIN_D3_DIR,DIO_PIN_D2_DIR,DIO_PIN_D1_DIR,DIO_PIN_D0_DIR);
}
 */


void MDIO_vSetPortDir(u8 A_u8PortNo, u8 A_u8Dir)
{
	switch (A_u8PortNo)
	{
	case PORT_A:
		DDRA=A_u8Dir;
		break;
	case PORT_B:
		DDRB=A_u8Dir;
		break;
	case PORT_C:
		DDRC=A_u8Dir;
		break;
	case PORT_D:
		DDRD=A_u8Dir;
		break;
	case PORT_E:
		DDRE=A_u8Dir;
		break;
	case PORT_F:
		DDRF=A_u8Dir;
		break;
	case PORT_G:
		DDRG=A_u8Dir;
		break;


	}
}
void MDIO_vSetPinDir(u8 A_u8PortNo, u8 A_u8PinNo, u8 A_u8Dir)
{
	if (A_u8Dir==DIO_OUTPUT)
	{
		switch (A_u8PortNo)
		{
		case PORT_A:
			SET_BIT(DDRA,A_u8PinNo);
			break;
		case PORT_B:
			SET_BIT(DDRB,A_u8PinNo);
			break;
		case PORT_C:
			SET_BIT(DDRC,A_u8PinNo);
			break;
		case PORT_D:
			SET_BIT(DDRD,A_u8PinNo);
			break;
		case PORT_E:
			SET_BIT(DDRE,A_u8PinNo);
			break;
		case PORT_F:
			SET_BIT(DDRF,A_u8PinNo);
			break;
		case PORT_G:
			SET_BIT(DDRG,A_u8PinNo);
			break;

		}
	}
	else if (A_u8Dir ==DIO_INPUT)
	{
		switch (A_u8PortNo)
		{
		case PORT_A:
			CLR_BIT(DDRA,A_u8PinNo);
			break;
		case PORT_B:
			CLR_BIT(DDRB,A_u8PinNo);
			break;
		case PORT_C:
			CLR_BIT(DDRC,A_u8PinNo);
			break;
		case PORT_D:
			CLR_BIT(DDRD,A_u8PinNo);
			break;
		case PORT_E:
			CLR_BIT(DDRE,A_u8PinNo);
			break;
		case PORT_F:
			CLR_BIT(DDRF,A_u8PinNo);
			break;
		case PORT_G:
			CLR_BIT(DDRG,A_u8PinNo);
			break;

		}
	}
}


void MDIO_vSetPinVal(u8 A_u8port,u8 A_u8pin,u8 A_u8value)
{
	switch (A_u8port)
	{
	case PORT_A:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTA,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTA,A_u8pin);
		}
		break;
	}
	case PORT_B:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTB,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTB,A_u8pin);
		}
		break;
	}
	case PORT_C:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTC,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTC,A_u8pin);
		}
		break;
	}
	case PORT_D:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTD,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTD,A_u8pin);
		}
		break;
	}
	case PORT_E:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTE,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTE,A_u8pin);
		}
		break;
	}
	case PORT_F:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTF,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTF,A_u8pin);
		}
		break;
	}
	case PORT_G:
	{
		if (A_u8value == DIO_HIGH)
		{
			SET_BIT(PORTG,A_u8pin);
		}
		else if (A_u8value == DIO_LOW)
		{
			CLR_BIT(PORTG,A_u8pin);
		}
		break;
	}
	}
}

u8 MDIO_u8ReadPin(u8 A_u8port,u8 A_u8pin)
{
	u8 L_u8ReturnVal ;
	switch (A_u8port)
	{
	case PORT_A:
	{
		L_u8ReturnVal=GET_BIT(PINA,A_u8pin);
		break;
	}
	case PORT_B:
	{
		L_u8ReturnVal=GET_BIT(PINB,A_u8pin);
		break;
	}
	case PORT_C:
	{
		L_u8ReturnVal=GET_BIT(PINC,A_u8pin);
		break;
	}
	case PORT_D:
	{
		L_u8ReturnVal=GET_BIT(PIND,A_u8pin);
		break;
	}
	case PORT_E:
	{
		L_u8ReturnVal=GET_BIT(PINE,A_u8pin);
		break;
	}
	case PORT_F:
	{
		L_u8ReturnVal=GET_BIT(PINF,A_u8pin);
		break;
	}
	case PORT_G:
	{
		L_u8ReturnVal=GET_BIT(PING,A_u8pin);
		break;
	}
	}
	return L_u8ReturnVal;
}

void MDIO_vTogglePin(u8 A_u8port,u8 A_u8pin)
{
	switch (A_u8port)
	{
	case PORT_A:
	{
		TOG_BIT(PORTA,A_u8pin);
		break;
	}
	case PORT_B:
	{
		TOG_BIT(PORTB,A_u8pin);
		break;
	}
	case PORT_C:
	{
		TOG_BIT(PORTC,A_u8pin);
		break;
	}
	case PORT_D:
	{
		TOG_BIT(PORTD,A_u8pin);
		break;
	}
	case PORT_E:
	{
		TOG_BIT(PORTE,A_u8pin);
		break;
	}
	case PORT_F:
	{
		TOG_BIT(PORTF,A_u8pin);
		break;
	}
	case PORT_G:
	{
		TOG_BIT(PORTG,A_u8pin);
		break;
	}
	}

}

void MDIO_vSetPortVal(u8 A_u8port ,u8 A_u8Value)
{
	switch (A_u8port)
	{
	case PORT_A:
	{
		PORTA = A_u8Value;
		break;
	}
	case PORT_B:
	{
		PORTB = A_u8Value;
		break;
	}
	case PORT_C:
	{
		PORTC = A_u8Value;
		break;
	}
	case PORT_D:
	{
		PORTD = A_u8Value;
		break;
	}
	case PORT_E:
	{
		PORTE = A_u8Value;
		break;
	}
	case PORT_F:
	{
		PORTF = A_u8Value;
		break;
	}
	case PORT_G:
	{
		PORTG = A_u8Value;
		break;
	}
	}
}

void DIO_voidPullUpState(u8 u8PortCpy , u8 u8PinCpy,u8 u8State)
{
	u8 u8Direction ;
	switch(u8PortCpy)
	{
	case PORT_A :
		u8Direction=GET_BIT(DDRA,u8PinCpy);
		break;
	case PORT_B :
		u8Direction=GET_BIT(DDRB,u8PinCpy);
		break;

	case PORT_C :
		u8Direction=GET_BIT(DDRC,u8PinCpy);
		break;

	case PORT_D :
		u8Direction= GET_BIT(DDRD,u8PinCpy);
		break;
	case PORT_E :
		u8Direction= GET_BIT(DDRE,u8PinCpy);
		break;
	case PORT_F :
		u8Direction= GET_BIT(DDRF,u8PinCpy);
		break;
	case PORT_G :
		u8Direction= GET_BIT(DDRG,u8PinCpy);
		break;
	}
	if (u8Direction==DIO_INPUT)
	{
		MDIO_vSetPinVal(u8PortCpy,u8PinCpy,u8State);
	}
	else
	{

		//do nothing
	}


}
