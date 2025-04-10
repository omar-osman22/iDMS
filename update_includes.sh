#!/bin/bash

# Function to update include paths in .c files
update_include_paths() {
    local file=$1
    sed -i 's|#include "PhoneList.h"|#include "../Application/PhoneList.h"|' "$file"
    sed -i 's|#include "SIM_interface.h"|#include "../Service/SIM_interface.h"|' "$file"
    sed -i 's|#include "TopWayLCD_Interface.h"|#include "../Service/TopWayLCD_Interface.h"|' "$file"
    sed -i 's|#include "LM35_Interface.h"|#include "../Service/LM35_Interface.h"|' "$file"
    sed -i 's|#include "DIO_Config.h"|#include "../MCAL/DIO/DIO_Config.h"|' "$file"
    sed -i 's|#include "DIO_Interface.h"|#include "../MCAL/DIO/DIO_Interface.h"|' "$file"
    sed -i 's|#include "DIO_Registers.h"|#include "../MCAL/DIO/DIO_Registers.h"|' "$file"
    sed -i 's|#include "ADC_Config.h"|#include "../MCAL/ADC/ADC_Config.h"|' "$file"
    sed -i 's|#include "ADC_Interface.h"|#include "../MCAL/ADC/ADC_Interface.h"|' "$file"
    sed -i 's|#include "ADC_Register.h"|#include "../MCAL/ADC/ADC_Register.h"|' "$file"
    sed -i 's|#include "UART_Interface.h"|#include "../MCAL/UART/UART_Interface.h"|' "$file"
    sed -i 's|#include "UART_Registers.h"|#include "../MCAL/UART/UART_Registers.h"|' "$file"
    sed -i 's|#include "EEPROM_Registers.h"|#include "../MCAL/EEPROM/EEPROM_Registers.h"|' "$file"
    sed -i 's|#include "INTERNAL_EEPROM.h"|#include "../MCAL/EEPROM/INTERNAL_EEPROM.h"|' "$file"
    sed -i 's|#include "BIT_MATH.h"|#include "../Utilities/BIT_MATH.h"|' "$file"
    sed -i 's|#include "STD_TYPES.h"|#include "../Utilities/STD_TYPES.h"|' "$file"
    sed -i 's|#include "errorStates.h"|#include "../Utilities/errorStates.h"|' "$file"
}

# Update include paths in all .c files
for file in $(find . -name "*.c"); do
    update_include_paths "$file"
done

echo "Include paths have been updated in all .c files."
