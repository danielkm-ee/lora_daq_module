#########################################
# Project setup
#########################################
TARGET = firmware
BUILD  = build
SRC_DIRS = src lib/STM32WLxx_HAL_Driver/Src
INC_DIRS = config \
			src \
		   lib/STM32WLxx_HAL_Driver/Inc \
		   lib/STM32WLxx/Include\
		   lib/CMSIS/Include
STARTUP = startup_stm32wle5xx.s

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -Wall -g -DSTM32WLE5xx -DUSE_HAL_DRIVER
CFLAGS += $(addprefix -I, $(INC_DIRS))
LDFLAGS = -T stm32wl.ld -Wl,--gc-sections
SRC = $(filter-out \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_alarm_template.c \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_wakeup_template.c, \
	$(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c)))
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o)))

all: $(BUILD)/$(TARGET).elf

$(BUILD)/$(TARGET).elf: $(OBJ) $(STARTUP)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BUILD)/$(TARGET).bin

# Build .c files from the src/ folder
$(BUILD)/%.o: src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Build .c files from the HAL driver folder
$(BUILD)/%.o: lib/STM32WLxx_HAL_Driver/Src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

	
clean:
	rm -rf $(BUILD)

printsrc:
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRC = $(SRC)"

flash:
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg -c "init; halt" -c "program build/firmware.elf verify reset"