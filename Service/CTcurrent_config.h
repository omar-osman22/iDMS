#ifndef CTCURRENT_CONFIG_H_
#define CTCURRENT_CONFIG_H_


#define VREF 4845 // Reference voltage for ADC in mV
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)
#define VOLTAGE_SOURCE 220.0 // Voltage source
#define NUMBER_OF_SAMPLES	1480 //�1480� the number of current samples over which you sample and average the current. This should be as close to a whole number of mains cycles as possible.

#define CALEBRATION	30.0 //Calibration parameter

#endif /* CTCURRENT_CONFIG_H_ */