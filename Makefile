# Compiler and tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# MCU and programmer settings
MCU = atmega128
PROGRAMMER = usbasp
PORT = /dev/ttyUSB0

# Directories
INCLUDE_DIRS = -I./Application -I./Application/inc -I./Service -I./MCAL/ADC -I./MCAL/DIO -I./MCAL/UART -I./MCAL/EEPROM -I./Utilities
SRC_DIRS = ./Application ./Application/src ./Service ./MCAL/ADC ./MCAL/DIO ./MCAL/UART ./MCAL/EEPROM

# Source files
SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))

# Object files
OBJS = $(SRCS:.c=.o)

# Output files
TARGET = main
HEX_TARGET = $(TARGET).hex

# Compiler flags
CFLAGS = -mmcu=$(MCU) $(INCLUDE_DIRS) -Os

# Linker flags
LDFLAGS = -mmcu=$(MCU)

# Compile and link
all: $(HEX_TARGET)
	@echo "Build successful!"

$(TARGET).elf: $(OBJS)
	@echo "Linking object files..."
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	@echo "Linking complete."

$(HEX_TARGET): $(TARGET).elf
	@echo "Creating hex file..."
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "Hex file created."

# Compile source files to object files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $<."

# Clean up
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(TARGET).elf $(HEX_TARGET)
	@echo "Clean complete."

# Upload to MCU
upload: $(HEX_TARGET)
	@echo "Uploading to MCU..."
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$(HEX_TARGET):i
	@echo "Upload complete."

.PHONY: all clean upload