# Compiler and tools
CC = avr-gcc
HOST_CC = gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
SIMULAVR = simavr

# MCU and programmer settings
MCU = atmega128
PROGRAMMER = usbasp
PORT = /dev/ttyUSB0

# Directories
INCLUDE_DIRS = -I./inc -I./Service -I./MCAL/ADC -I./MCAL/DIO -I./MCAL/UART -I./MCAL/EEPROM -I./Utilities -I./Config -I./MCAL/Delay
SRC_DIRS = ./src ./Service ./MCAL/ADC ./MCAL/DIO ./MCAL/UART ./MCAL/EEPROM ./Config

# Source files
SRCS = main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Object files
OBJS = $(patsubst %.c,build/%.o,$(SRCS))

# Output files
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
HEX_TARGET = $(TARGET).hex

# Compiler flags
CFLAGS = -mmcu=$(MCU) $(INCLUDE_DIRS) -Os -DF_CPU=16000000UL -DTARGET_AVR -Wall -Wextra

# Debug flags
DEBUG_FLAGS = -DDEBUG_ENABLE -g -O0

# Linker flags
LDFLAGS = -mmcu=$(MCU)

# Compile and link
all: $(BUILD_DIR) $(HEX_TARGET)
	@echo "Build successful!"

# Debug build with logging enabled
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all
	@echo "Debug build successful!"

# Test build
test: $(BUILD_DIR)
	@echo "Running tests..."
	$(HOST_CC) -DTARGET_HOST -o $(BUILD_DIR)/test_runner Test/test_phone_list_stub.c
	@echo "Tests passed (placeholder)."

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/src
	mkdir -p $(BUILD_DIR)/Service
	mkdir -p $(BUILD_DIR)/MCAL/ADC
	mkdir -p $(BUILD_DIR)/MCAL/DIO
	mkdir -p $(BUILD_DIR)/MCAL/UART
	mkdir -p $(BUILD_DIR)/MCAL/EEPROM
	mkdir -p $(BUILD_DIR)/Config

$(TARGET).elf: $(OBJS)
	@echo "Linking object files..."
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	@echo "Linking complete."

$(HEX_TARGET): $(TARGET).elf
	@echo "Creating hex file..."
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "Hex file created."

build/%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $<."

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)
	@echo "Clean complete."

.PHONY: all clean debug test
