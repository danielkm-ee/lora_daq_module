#########################################
# Project setup
#########################################
TARGET = firmware # Name of the output bin files
BUILD  = build # folder for compiled outputs
SRC_DIRS = src lib/STM32WLxx_HAL_Driver/Src # Source code folders; main and HAL driver
INC_DIRS = config \
			src \
           lib/STM32WLxx_HAL_Driver/Inc \
           lib/STM32WLxx/Include\
		   lib/CMSIS/Include # Include header files directories
STARTUP = startup_stm32wle5xx.s # Startup assembly file, using template at lib/CMSIS/Device/ST/STM32WLxx/Source/Templates

# Toolchain

CC = arm-none-eabi-gcc # cross-compiler for ARM Cortex-M
OBJCOPY = arm-none-eabi-objcopy # to convert ELF to binary
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -Wall -g -DSTM32WLE5xx -DUSE_HAL_DRIVER # Compiler flags
CFLAGS += $(addprefix -I, $(INC_DIRS))
LDFLAGS = -T stm32wl.ld -Wl,--gc-sections # using Linter script stm32wl.ld; remove unused sections
SRC = $(filter-out \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_alarm_template.c \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_wakeup_template.c, \
	$(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c))) # All .c source files, excluding unused HAL timebase templates
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o))) # Object files to be built

all: $(BUILD)/$(TARGET).elf

$(BUILD)/$(TARGET).elf: $(OBJ) $(STARTUP)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BUILD)/$(TARGET).bin
	@echo
	@echo "Build complete: $(BUILD)/$(TARGET).elf and $(BUILD)/$(TARGET).bin"


# Build .c files from the src/ folder
$(BUILD)/%.o: src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Build .c files from the HAL driver folder
$(BUILD)/%.o: lib/STM32WLxx_HAL_Driver/Src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(BUILD)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg \
		-c "init; halt" \
		-c "program $< verify reset exit"

clean:
	rm -rf $(BUILD)

printsrc:
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRC = $(SRC)"

flash: $(BUILD)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg \
	  -c "adapter speed 100" \
	  -c "init; halt" \
	  -c "program $(BUILD)/$(TARGET).elf verify reset exit"