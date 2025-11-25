#########################################
# Project setup
#########################################
TARGET = firmware
BUILD  = build
SRC_DIRS = src \
           src/tasks \
           src/radio/App \
		   src/radio/Target \
           lib/STM32WLxx_HAL_Driver/Src \
           lib/FreeRTOS/Source \
           lib/FreeRTOS/Source/CMSIS_RTOS \
           lib/FreeRTOS/Source/portable/GCC/ARM_CM3 \
           lib/FreeRTOS/Source/portable/MemMang \
           lib/SubGHz_Phy/stm32_radio_driver \
           lib/STM32WLxx_LoRa_E5_mini \
           Utilities/lpm/tiny_lpm \
           Utilities/misc \
           Utilities/sequencer \
           Utilities/timer \
           Utilities/trace/adv_trace \
		   lib/LoRaWAN/Mac \
           lib/LoRaWAN/Mac/Region \
           lib/LoRaWAN/Crypto \
           lib/LoRaWAN/Utilities \
           lib/LoRaWAN/LmHandler \

INC_DIRS = config \
           src \
           src/tasks \
           src/radio/App \
		   src/radio/Target \
           lib/STM32WLxx_HAL_Driver/Inc \
           lib/STM32WLxx/Include \
           lib/CMSIS/Include \
           lib/FreeRTOS/Source/include \
           lib/FreeRTOS/Source/CMSIS_RTOS \
           lib/FreeRTOS/Source/portable/GCC/ARM_CM3 \
           lib/SubGHz_Phy \
           lib/SubGHz_Phy/common \
           lib/SubGHz_Phy/stm32_radio_driver \
           lib/STM32WLxx_LoRa_E5_mini \
           Utilities/lpm/tiny_lpm \
           Utilities/misc \
           Utilities/sequencer \
           Utilities/timer \
           Utilities/trace/adv_trace \
		   lib/LoRaWAN/Mac \
           lib/LoRaWAN/Mac/Region \
           lib/LoRaWAN/Crypto \
           lib/LoRaWAN/Utilities \
           lib/LoRaWAN/LmHandler \
		   lib/LoRaWAN/LmHandler/Packages
STARTUP = startup_stm32wle5xx.s


CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O2 -Wall -g -DSTM32WLE5xx -DUSE_HAL_DRIVER -DLORAWAN_COMPLIANCE_TEST=0
CFLAGS += $(addprefix -I, $(INC_DIRS))
LDFLAGS = -T stm32wl.ld -Wl,--gc-sections
SRC = $(filter-out \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_alarm_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_wakeup_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_tim_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_msp_template.c, \
			$(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c)))
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o)))

all: $(BUILD)/$(TARGET).elf
$(BUILD)/$(TARGET).elf: $(OBJ) $(STARTUP)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BUILD)/$(TARGET).bin
# 1. Main Application
$(BUILD)/%.o: src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/tasks/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/radio/App/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/radio/Target/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# 2. HAL Drivers
$(BUILD)/%.o: lib/STM32WLxx_HAL_Driver/Src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# 3. FreeRTOS (Core + CMSIS + Portable)
$(BUILD)/%.o: lib/FreeRTOS/Source/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/FreeRTOS/Source/CMSIS_RTOS/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/FreeRTOS/Source/portable/GCC/ARM_CM3/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/FreeRTOS/Source/portable/MemMang/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# 4. Radio Middleware (SubGHz Phy)
$(BUILD)/%.o: lib/SubGHz_Phy/stm32_radio_driver/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Utilities (LPM, Timer, etc.)
$(BUILD)/%.o: Utilities/lpm/tiny_lpm/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/misc/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/timer/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/sequencer/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: Utilities/trace/adv_trace/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# 6. Board Support (Wio-E5 Mini)
$(BUILD)/%.o: lib/STM32WLxx_LoRa_E5_mini/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/LoRaWAN/Mac/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/LoRaWAN/Mac/Region/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/LoRaWAN/Crypto/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/LoRaWAN/Utilities/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/LoRaWAN/LmHandler/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: lib/LoRaWAN/LmHandler/Packages/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

printsrc:
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRC = $(SRC)"