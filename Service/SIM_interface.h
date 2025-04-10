/*
 * SIM_interface.h
 *
 *  Created on: Feb 14, 2022
 *      Author: pc
 */

#ifndef HAL_SIM800L_SIM_INTERFACE_H_
#define HAL_SIM800L_SIM_INTERFACE_H_

/* MACROS */

#define	SIM_MAX_RESPONSE_LINES		5
#define	SIM_MAX_RESPONSE_COLUMN		100

#define	SIM_RESPONSE_LINE_1		0
#define	SIM_RESPONSE_LINE_2		1
#define	SIM_RESPONSE_LINE_3		2
#define	SIM_RESPONSE_LINE_4		3
#define	SIM_RESPONSE_LINE_5		4

#define	SIM_RESPONSE_COLUMN_1		0


/* SHARED VARIABLE */
extern u8 SIM_u8Response[SIM_MAX_RESPONSE_LINES][SIM_MAX_RESPONSE_COLUMN];	// the second line after repeated AT command


/* FUNCTION PROTOTYPE */
void 	SIM_voidSendCommand(u8 const *Copy_u8ATCommand);
void 	SIM_voidInit(void);
ES_t 	SIM_estCheckIfReady(void);
ES_t 	SIM_estCheckIfRegistered(void);
ES_t 	SIM_estGetResponse(void);
ES_t 	SIM_estSendSMS(u8 *Copy_u8PhoneNumber, u8 *Copy_u8SMS);
ES_t	SIM_estSendLocation(u8 *Copy_u8PhoneNumber, u8 *Copy_u8Latitude, u8 *Copy_u8Longitude);
ES_t 	SIM_estMakeCall(u8 *Copy_u8PhoneNumber);

void 	SIM_SendSMS(u8 const *Copy_u8PhoneNumber, u8 const *Copy_u8SMS);
void 	SIM_MakeCall(u8 *Copy_u8PhoneNumber);


#endif /* HAL_SIM800L_SIM_INTERFACE_H_ */
