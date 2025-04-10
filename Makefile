# Compiler and tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
SIMULAVR = simavr

# MCU and programmer settings
MCU = atmega128
PROGRAMMER = usbasp
PORT = /dev/ttyUSB0

# Directories
INCLUDE_DIRS = -I./Application -I./Application/inc -I./Service -I./MCAL/ADC -I./MCAL/DIO -I./MCAL/UART -I./MCAL/EEPROM -I./Utilities -I./Utilities/Debug
SRC_DIRS = ./Application ./Application/src ./Service ./MCAL/ADC ./MCAL/DIO ./MCAL/UART ./MCAL/EEPROM ./Utilities/Debug

# Source files
SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))

# Object files
OBJS = $(SRCS:.c=.o)

# Output files
TARGET = main
HEX_TARGET = $(TARGET).hex

# Compiler flags
CFLAGS = -mmcu=$(MCU) $(INCLUDE_DIRS) -Os

# Debug flags - add when debugging
DEBUG_FLAGS = -DDEBUG_MODE -g

# Linker flags
LDFLAGS = -mmcu=$(MCU)

# Compile and link
all: $(HEX_TARGET)
	@echo "Build successful!"

# Debug build with logging enabled
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(HEX_TARGET)
	@echo "Debug build successful!"

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

# Simulate the AVR program with output to terminal
simulate: $(TARGET).elf
	@echo "Starting AVR simulation..."
	$(SIMULAVR) -d $(MCU) -f $(TARGET).elf -W 0x20,- -R 0x22,- -T exit
	@echo "Simulation complete."

# Monitor serial output from MCU
monitor:
	@echo "Starting serial monitor on $(PORT)..."
	screen $(PORT) 9600
	@echo "Serial monitor closed."

.PHONY: all clean upload debug simulate monitor