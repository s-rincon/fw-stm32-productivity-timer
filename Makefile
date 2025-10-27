##########################################################
# Makefile for STM32 Productivity Timer
# Author: Fastbit Embedded Academy
##########################################################

# Project name
PROJECT = productivity_timer

# Build directory
BUILD_DIR = build

# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size

# Source files
C_SOURCES = \
	src/main.c \
	src/timer.c \
	src/gpio.c \
	src/system_stm32f4xx.c \
	src/startup_stm32f4xx.c

# Include directories
INCLUDES = -Iinc

# MCU flags
MCU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft

# Compiler flags
CFLAGS = $(MCU_FLAGS) $(INCLUDES)
CFLAGS += -Wall -Wextra
CFLAGS += -O0 -g3
CFLAGS += -ffunction-sections -fdata-sections

# Linker flags
LDFLAGS = $(MCU_FLAGS)
LDFLAGS += -T STM32F407VG.ld
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map
LDFLAGS += --specs=nosys.specs

# Object files
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))

# Default target
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C sources
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) -c $(CFLAGS) $< -o $@

# Link
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Generate hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating hex file"
	$(OBJCOPY) -O ihex $< $@

# Generate bin file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating bin file"
	$(OBJCOPY) -O binary $< $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Flash (requires st-flash utility from stlink tools)
flash: $(BUILD_DIR)/$(PROJECT).bin
	st-flash write $(BUILD_DIR)/$(PROJECT).bin 0x08000000

.PHONY: all clean flash
