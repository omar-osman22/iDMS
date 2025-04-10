#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_


#define Channel_A0 0
#define Channel_A1 1
#define Channel_A2 2
#define Channel_A3 3
#define Channel_A4 4
#define Channel_A5 5
#define Channel_A6 6
#define Channel_A7 7

#define AREF 0
#define AVCC 1
#define REVERCED 2
#define INTERNAL 3

#define OFF 0
#define ON 1

#define VOL_REF_SEL AVCC
/*
options:
AREF
AVCC
REVERCED
INTERNAL
*/
#define LEFT_ADJUST_MODE OFF
/*
options:
OFF
ON
*/

#define AUTO_TRIGGER_MODE OFF
/*
options:
OFF
ON
*/
#define ADC_ENABLE_INTERRUPT ON
/*
options:
OFF
ON
*/
#define ADC_ENABLE ON
/*
options:
OFF
ON
*/

void ADC_Init (void); // ADC Initialization And Enable
u16 ADC_Read_senc (u8 channel); // Read From The ADC Channel
void ADC_Read_Asenc (u8 channel); // using interrupt
void ADC_VID_SET_CALL_BACK(void (*PTR)(void));
u16 ADC_return (void);
#endif /* ADC_INTERFACE_H_ */
