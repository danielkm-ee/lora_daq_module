# Usage:
#   make           # Build transmitter (default)
#   make tx        # Build transmitter
#   make rx        # Build receiver
#   make flash     # Flash transmitter (default)
#   make flash_tx  # Flash transmitter
#   make flash_rx  # Flash receiver
#   make tx BOARD=CUSTOM    # Build TX for custom PCB
#   make rx BOARD=CUSTOM    # Build RX for custom PCB


MODE ?= TX
BOARD ?= WIO_E5

# CFlags need to declare eariler
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -Wall -g -DSTM32WLE5xx -DUSE_HAL_DRIVER

# Board Selection
ifeq ($(BOARD),WIO_E5)
    CFLAGS += -DBOARD_WIO_E5
else ifeq ($(BOARD),CUSTOM)
    CFLAGS += -DBOARD_CUSTOM
else
    $(error Unknown BOARD specified: $(BOARD). Valid options are WIO_E5 or CUSTOM)
endif

# Mode Selection
ifeq ($(MODE),TX)
    CFLAGS += -DBUILD_TX
    TARGET = firmware_tx_$(BOARD)
else ifeq ($(MODE),RX)
    CFLAGS += -DBUILD_RX
    TARGET = firmware_rx_$(BOARD)
endif

#########################################
# Project setup
#########################################
BUILD  = build
# Conditional source directories based on mode
ifeq ($(MODE),TX)
    SRC_DIRS = src \
               src/app \
               src/i2c \
               src/lora \
               src/power \
               src/scheduling \
               src/uart \
               src/sensors \
               src/gpio \
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
               src/sensors \
               src/gpio \
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
else ifeq ($(MODE),RX)
    # RX doesn't need I2C, sensors, scheduling
    SRC_DIRS = src \
               src/app \
               src/lora \
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
               src/lora \
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
endif

STARTUP = startup_stm32wle5xx.s

CFLAGS += $(addprefix -I, $(INC_DIRS))
LDFLAGS = -T stm32wl.ld -Wl,--gc-sections
ifeq ($(MODE),TX)
    # Exclude main_rx.c when building TX
    SRC = $(filter-out \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_alarm_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_wakeup_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_tim_template.c \
        src/app/main_rx.c, \
        $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c)))
else ifeq ($(MODE),RX)
    # Exclude main_tx.c when building RX
    SRC = $(filter-out \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_alarm_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_rtc_wakeup_template.c \
        lib/STM32WLxx_HAL_Driver/Src/stm32wlxx_hal_timebase_tim_template.c \
        src/app/main_tx.c, \
        $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c)))
endif
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o)))

all: $(BUILD)/$(TARGET).elf

$(BUILD)/$(TARGET).elf: $(OBJ) $(STARTUP)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BUILD)/$(TARGET).bin
	@echo "Build complete: $(TARGET)"
	@echo "Binary: $(BUILD)/$(TARGET).bin"
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

$(BUILD)/%.o: src/sensors/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/gpio/%.c
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

tx:
	@$(MAKE) all MODE=TX BOARD=$(BOARD)

rx:
	@$(MAKE) all MODE=RX BOARD=$(BOARD)

flash: tx
	@echo "Flashing $(TARGET)..."
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg \
		-c "init; halt" \
		-c "program build/$(TARGET).elf verify reset exit"

flash_tx: tx
	@echo "Flashing transmitter for $(BOARD)..."
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg \
		-c "init; halt" \
		-c "program build/firmware_tx_$(BOARD).elf verify reset exit"

flash_rx: rx
	@echo "Flashing receiver for $(BOARD)..."
	openocd -f interface/stlink.cfg -f target/stm32wlx.cfg \
		-c "init; halt" \
		-c "program build/firmware_rx_$(BOARD).elf verify reset exit"

clean:
	rm -rf $(BUILD)

printsrc:
	@echo "MODE = $(MODE)"
	@echo "BOARD = $(BOARD)"
	@echo "TARGET = $(TARGET)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "SRC_DIRS = $(SRC_DIRS)"

format:
	clang-format -i $(shell find src config -type f \( -name '*.c' -o -name '*.h' \))

.PHONY: all tx rx flash flash_tx flash_rx clean printsrc format