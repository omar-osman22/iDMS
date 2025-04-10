/*
 * ADC_program.c
 *
 *  Created on: Dec 18, 2023
 *      Author: Ahmed Samy
 */
#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include"../DIO/DIO_interface.h"
#include"ADC_register.h"
#include"ADC_interface.h"



void (*ADC_CALLBACK_PTR)(void)=NULL;


void ADC_Init (void)
{
	//select your voltage reffrence
	#if VOL_REF_SEL== AREF

	CLR_BIT(ADMUX,REFS0);
	CLR_BIT(ADMUX,REFS1);

	#elif VOL_REF_SEL== AVCC

	SET_BIT(ADMUX,REFS0);
	CLR_BIT(ADMUX,REFS1);

	#elif VOL_REF_SEL== REVERCED

	CLR_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);

	#elif VOL_REF_SEL== INTERNAL

	SET_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);

	#endif

	// select left adjust mode
	#if LEFT_ADJUST_MODE== OFF

	CLR_BIT(ADMUX,ADLAR);

	#elif LEFT_ADJUST_MODE== ON

	SET_BIT(ADMUX,ADLAR);

    #endif

	//select auto trigger mode
	#if AUTO_TRIGGER_MODE== OFF

	CLR_BIT(ADCSRA,ADFR);

	#elif AUTO_TRIGGER_MODE== ON

	SET_BIT(ADCSRA,ADATE);

	#endif

	//prescaller
	SET_BIT(ADCSRA,ADPS0);
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);

	// Enabling adc interrupt
	#if ADC_ENABLE_INTERRUPT== ON
	SET_BIT(ADCSRA,ADIE);
	#elif ADC_ENABLE_INTERRUPT==of
	CLR_BIT(ADCSRA,ADIE);
	#endif
	// ADC enabling
	#if ADC_ENABLE== ON
	SET_BIT(ADCSRA,ADEN);
	#elif ADC_ENABLE== OFF
	CLR_BIT(ADCSRA,ADEN);
	#endif

}
u16 ADC_Read_senc (u8 channel)
{
	// selcting the MUX by BIT MASKING
	ADMUX=((ADMUX&0b11100000)|(channel));
	//START CONVERSION
	SET_BIT(ADCSRA,ADSC);

	//pooling in the flag value
	while((GET_BIT(ADCSRA,ADIF))==0);

	// setting the flag to 0 because i didnot use ISR
	SET_BIT(ADCSRA,ADIF);

	return ADC_Reg;
}



void ADC_Read_Asenc (u8 channel)
{
	ADMUX=((ADMUX&0b11100000)|(channel));
	SET_BIT(ADCSRA,ADSC);
}





void ADC_VID_SET_CALL_BACK(void (*PTR)(void)){
	ADC_CALLBACK_PTR=PTR;
}


void __vector_16 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_16 (void){
	ADC_CALLBACK_PTR();
}
