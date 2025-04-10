#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/ADC/ADC_interface.h"
#include "CTcurrent_interface.h"
#include "CTcurrent_config.h"
#include "math.h"

// Global variables
f32 Irms, ICAL, offsetI, filteredI;
u8 sampleI;
f32 sqI, sumI;


void CT_vInit(void)
{
    // Calibration constant for the sensor
    ICAL = CALEBRATION;

    // ADC offset, assuming a mid-scale value (for our 10-bit ADC, this would be 512)
    offsetI = ADC_COUNTS >> 1;
}


f32 CT_f32CalcIrms(u8 ADC_Channel)
{
    // Loop through the specified number of samples
    for (u16 n = 0; n < NUMBER_OF_SAMPLES; n++)
    {
        // Read the current sensor value from the specified ADC channel
        sampleI = ADC_Read_senc(ADC_Channel);

        // Digital low-pass filter to extract the DC offset (centered on 0 counts)
        offsetI = (offsetI + (sampleI - offsetI) / 1024);
        filteredI = sampleI - offsetI;

        // Square the filtered current value
        sqI = filteredI * filteredI;
        // Accumulate the squared values
        sumI += sqI;
    }

    // Calculate the current ratio based on the calibration constant and supply voltage
    f32 I_RATIO = ICAL * ((VREF / 1000.0) / (ADC_COUNTS));
    // Calculate the RMS current value
    Irms = I_RATIO * sqrt(sumI / NUMBER_OF_SAMPLES);

    // Reset the sum accumulator
    sumI = 0;

    return Irms;
}


f32 CT_f32CalcPower(f32 Irms_Value)
{
    // Calculate power using the RMS current value and the source voltage
    f32 f32_Power = Irms_Value * VOLTAGE_SOURCE;
    return f32_Power;
}

