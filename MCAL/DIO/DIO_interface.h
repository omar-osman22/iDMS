/*****************************************************************/
/* Author  : Ahmed Samy                                               */
/* SWC     : DIO                                                 */
/* Version : 1.0                                                 */
/* Date    : 07 Dec 2023                                         */
/*****************************************************************/
#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_


#define DIO_LOW 0
#define DIO_HIGH 1

#define DIO_INPUT 0
#define DIO_OUTPUT 1

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4
#define PORT_F 5
#define PORT_G 6

#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4
#define PIN_5 5
#define PIN_6 6
#define PIN_7 7

//configure directions and pull up
void MDIO_vInt (void);

void MDIO_vSetPortDir(u8 A_u8PortNo, u8 A_u8Dir);
void MDIO_vSetPinDir(u8 A_u8PortNo, u8 A_u8PinNo, u8 A_u8Dir);

void MDIO_vSetPinVal (u8 A_u8port , u8 A_u8pin , u8 A_u8value);

u8 MDIO_u8ReadPin (u8 A_u8port , u8 A_u8pin);

void MDIO_vTogglePin(u8 A_u8port , u8 A_u8pin);

void MDIO_vSetPortVal(u8 A_u8port , u8 A_u8Value);

void DIO_voidPullUpState(u8 u8PortCpy , u8 u8PinCpy,u8 u8State);

#endif
