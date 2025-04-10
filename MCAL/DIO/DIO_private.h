/*****************************************************************/
/* Author  : Ahmed Samy                                               */
/* SWC     : DIO                                                 */
/* Version : 1.0                                                 */
/* Date    : 07 Dec 2023                                         */
/*****************************************************************/
#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_


#define CONCAT(p7,p6,p5,p4,p3,p2,p1,p0)    CONCAT_2(p7,p6,p5,p4,p3,p2,p1,p0)
#define CONCAT_2(p7,p6,p5,p4,p3,p2,p1,p0)  0b##p7##p6##p5##p4##p3##p2##p1##p0




//DDR REGISTERS ADDRESSES
#define DDRA  *( (volatile u8*) 0x3A)
#define DDRB  *( (volatile u8*) 0x37)
#define DDRC  *( (volatile u8*) 0x34)
#define DDRD  *( (volatile u8*) 0x31)
#define DDRE  *( (volatile u8*) 0x22)
#define DDRF  *( (volatile u8*) 0x61)
#define DDRG  *( (volatile u8*) 0x64)

//PORT REGISTERS ADDRESSES
#define PORTA *( (volatile u8*) 0x3B)
#define PORTB *( (volatile u8*) 0x38)
#define PORTC *( (volatile u8*) 0x35)
#define PORTD *( (volatile u8*) 0x32)
#define PORTE *( (volatile u8*) 0x23)
#define PORTF *( (volatile u8*) 0x62)
#define PORTG *( (volatile u8*) 0x65)

//PIN REGISTERS ADDRESSES
#define PINA  *( (volatile u8*) 0x39)
#define PINB  *( (volatile u8*) 0x36)
#define PINC  *( (volatile u8*) 0x33)
#define PIND  *( (volatile u8*) 0x30)
#define PINE  *( (volatile u8*) 0x21)
#define PINF  *( (volatile u8*) 0x20)
#define PING  *( (volatile u8*) 0x63)

#endif
