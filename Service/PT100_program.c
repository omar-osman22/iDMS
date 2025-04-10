/*
 * PT100_program.c
 *
 *  Created on: 21 May 2024
 *      Author: Ahmed Samy
 */

#include "../Utilities/STD_TYPES.h"
#include "../Utilities/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/ADC/ADC_interface.h"
#include "PT100_interface.h"
#include "PT100_config.h"
#include "util/delay.h"


// Define known points for voltage and temperature
const f32 voltages[] = { 0.079, 0.56, 1.04, 1.99, 2.95 };    // Voltages in Volts
const f32 temperatures[] = { 0, 25.0, 50.0, 100.0, 150.0 };  // Temperatures in Celsius

// Number of data points
const s16 numPoints = sizeof(voltages) / sizeof(voltages[0]);

/**
 * @brief Perform linear interpolation to find a value
 * @param x The value to interpolate for
 * @param x0 The lower bound of the x value
 * @param x1 The upper bound of the x value
 * @param y0 The lower bound of the y value
 * @param y1 The upper bound of the y value
 * @return The interpolated y value
 */
static f32 PT100_f32Interpolate(f32 x, f32 x0, f32 x1, f32 y0, f32 y1) {
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

f32 PT100_f32CalculateTemperature(u8 ADC_channel) {
    f32 f32_temp = 0;
    // Read the sensor value from the specified ADC channel
    s16 sensorValue = ADC_Read_senc(ADC_channel);
    // Convert the ADC value to a voltage
    f32 voltage = sensorValue * ((VREF / 1000.0) / 1023.0);

    // If the voltage is higher than the maximum known point, set the temperature to 0�C
    if (voltage > voltages[numPoints - 1]) {
        return 0;
    }

    // Find the appropriate interval for interpolation
    s16 i = 0;
    while (voltage > voltages[i + 1] && i < numPoints - 1) {
        i++;
    }

    // Perform interpolation to calculate the temperature
    f32_temp = PT100_f32Interpolate(voltage, voltages[i], voltages[i + 1], temperatures[i], temperatures[i + 1]);

    // Apply offset if any (edit it in the config file)
    return (f32_temp + offset);
}

