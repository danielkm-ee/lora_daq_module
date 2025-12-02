#########################################
# Project setup
#########################################
TARGET = firmware
BUILD  = build
SRC_DIRS = src \
		   src/app \
		   src/i2c \
		   src/lora \
		   src/power \
		   src/scheduling \
		   src/uart \
           lib/STM32WLxx_HAL_Driver/Src \
           lib/SubGHz_Phy \
           lib/SubGHz_Phy/stm32_radio_driver \
           lib/BSP/STM32WLxx_LoRa_E5_mini \
           Utilities/timer \
           Utilities/misc \
           Utilities/trace/adv_trace \
           Utilities/lpm/tiny_lpm

INC_DIRS = config \
           src \
		   src/app \
		   src/i2c \
		   src/lora \
		   src/power \
		   src/scheduling \
		   src/uart \
           lib/STM32WLxx_HAL_Driver/Inc \
           lib/STM32WLxx/Include \
           lib/CMSIS/Include \
           lib/SubGHz_Phy \
           lib/SubGHz_Phy/stm32_radio_driver \
           lib/BSP/STM32WLxx_LoRa_E5_mini \
           Utilities/timer \
           Utilities/misc \
           Utilities/trace/adv_trace \
           Utilities/lpm/tiny_lpm
STARTUP = startup_stm32wle5xx.s

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -Wall -g -DSTM32WLE5xx -DUSE_HAL_DRIVER
CFLAGS += $(addprefix -I, $(INC_DIRS))
LDFLAGS = -T stm32wl.ld -Wl,--gc-sections
SRC = $(filter-out \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_alarm_template.c \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_wakeup_template.c \
	lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_tim_template.c, \
	$(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c)))
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o)))

all: $(BUILD)/$(TARGET).elf

$(BUILD)/$(TARGET).elf: $(OBJ) $(STARTUP)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BUILD)/$(TARGET).bin

# Build .c files from any source directory

$(BUILD)/%.o: src/app/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/i2c/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/lora/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/power/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/scheduling/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/uart/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/STM32WLxx_HAL_Driver/Src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/SubGHz_Phy/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/BSP/STM32WLxx_LoRa_E5_mini/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/timer/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/misc/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/SubGHz_Phy/stm32_radio_driver/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/trace/adv_trace/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/lpm/tiny_lpm/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

printsrc:
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRC = $(SRC)"

flash:
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg -c "init; halt" -c "program build/firmware.elf verify reset"

format:
	clang-format -i $(shell find src config -type f \( -name '*.c' -o -name '*.h' \))
