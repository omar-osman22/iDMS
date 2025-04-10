#ifndef CTCURRENT_INTERFACE_H_
#define CTCURRENT_INTERFACE_H_

/**
 * @brief Initialize the current sensor parameters
 */
void CT_vInit(void);

/**
 * @brief Calculate the RMS current value from the ADC samples
 * @param ADC_Channel The ADC channel to read the current sensor value from
 * @return The calculated RMS current value
 */
f32 CT_f32CalcIrms(u8 ADC_Channel);

/**
 * @brief Calculate the power based on the RMS current value
 * @param Irms_Value The RMS current value
 * @return The calculated power
 */
f32 CT_f32CalcPower(f32 Irms_Value);

#endif /* CTCURRENT_INTERFACE_H_ */
