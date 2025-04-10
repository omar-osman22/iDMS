#!/bin/bash

# Create directories
mkdir -p Application Service MCAL/DIO MCAL/ADC MCAL/UART MCAL/EEPROM Utilities Config Debug

# Move files to Application layer
mv main.c Application/
mv PhoneList.c Application/
mv PhoneList.h Application/

# Move files to Service layer
mv SIM_interface.h Service/
mv SIM_prog.c Service/
mv TopWayLCD_Interface.h Service/
mv TopWayLCD_Program.c Service/
mv LM35_Interface.h Service/
mv LM35_Program.c Service/

# Move files to MCAL layer
mv DIO_Config.h MCAL/DIO/
mv DIO_Interface.h MCAL/DIO/
mv DIO_Registers.h MCAL/DIO/
mv DIO.c MCAL/DIO/

mv ADC_Config.h MCAL/ADC/
mv ADC_Interface.h MCAL/ADC/
mv ADC_Program.c MCAL/ADC/
mv ADC_Register.h MCAL/ADC/

mv UART_Interface.h MCAL/UART/
mv UART_Prog.c MCAL/UART/
mv UART_Registers.h MCAL/UART/

mv EEPROM_Registers.h MCAL/EEPROM/
mv INTERNAL_EEPROM.c MCAL/EEPROM/
mv INTERNAL_EEPROM.h MCAL/EEPROM/

# Move files to Utilities layer
mv BIT_MATH.h Utilities/
mv STD_TYPES.h Utilities/
mv errorStates.h Utilities/

# Move configuration files to Config layer
mv .cproject Config/
mv .gitignore Config/
mv .project Config/
mv .settings Config/

# Move debug files to Debug layer
mv Debug/* Debug/
mv README.md Debug/

echo "Files have been organized into a layered architecture."
