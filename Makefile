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
INCLUDE_DIRS = -I./Application -I./Application/inc -I./Service -I./MCAL/ADC -I./MCAL/DIO -I./MCAL/UART -I./MCAL/EEPROM -I./Utilities -I./Utilities/Debug -I./Debug -I./Test -I./Config
SRC_DIRS = ./Application ./Application/src ./Service ./MCAL/ADC ./MCAL/DIO ./MCAL/UART ./MCAL/EEPROM ./Debug ./Test ./Config

# Source files
SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))

# Object files
OBJS = $(SRCS:.c=.o)

# Output files
TARGET = main
HEX_TARGET = $(TARGET).hex
TEST_TARGET = phone_list_test

# Test specific source files (excluding main application)
TEST_SRCS = ./Test/test_phone_list.c ./Test/test_phone_list_stub.c ./Service/PhoneList.c \
            ./MCAL/UART/UART_Prog.c ./MCAL/EEPROM/INTERNAL_EEPROM.c ./Debug/debug_log.c

# Test specific object files
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Compiler flags
CFLAGS = -mmcu=$(MCU) $(INCLUDE_DIRS) -Os -DF_CPU=16000000UL

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

# Build only the phone list test program
test: CFLAGS += $(DEBUG_FLAGS)
test: $(TEST_OBJS)
	@echo "Building phone list test program..."
	$(CC) $(LDFLAGS) $(TEST_OBJS) -o $(TEST_TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $(TEST_TARGET).elf $(TEST_TARGET).hex
	@echo "Phone list test program built successfully!"

# Run the phone list test program in simulation mode
test_simulate: test
	@echo "Starting test simulation with simavr..."
	$(SIMULAVR) -m $(MCU) -f $(TEST_TARGET).elf
	@echo "Test simulation completed."

# Clean up
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(TARGET).elf $(HEX_TARGET) $(TEST_TARGET).elf $(TEST_TARGET).hex
	@echo "Clean complete."

# Upload to MCU
upload: $(HEX_TARGET)
	@echo "Uploading to MCU..."
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$(HEX_TARGET):i
	@echo "Upload complete."

# Upload test program to MCU
upload_test: test
	@echo "Uploading test program to MCU..."
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$(TEST_TARGET).hex:i
	@echo "Test program uploaded."

# Simulate the AVR program with output to terminal using simavr
simulate: $(TARGET).elf
	@echo "Starting AVR simulation with simavr..."
	$(SIMULAVR) -m $(MCU) -f $(TARGET).elf
	@echo "Simulation complete."

# Monitor serial output from MCU
monitor:
	@echo "Starting serial monitor on $(PORT)..."
	screen $(PORT) 9600
	@echo "Serial monitor closed."

.PHONY: all clean upload debug simulate monitor test test_simulate upload_test