/*
 * PT100_interface.h
 *
 *  Created on: 21 May 2024
 *      Author: Ahmed Samy
 */

#ifndef PT100_INTERFACE_H_
#define PT100_INTERFACE_H_

/**
 * @brief Calculate the temperature from the RTD sensor voltage
 * @param ADC_channel The ADC channel to read the RTD sensor value from
 * @return The calculated temperature in Celsius
 */
f32 PT100_f32CalculateTemperature(u8 ADC_channel);

#endif /* PT100_INTERFACE_H_ */
