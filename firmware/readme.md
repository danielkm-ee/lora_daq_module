# LoRA DAQ Firmware

## Toolchain Setup

This project targets the **STM32WLE5** (Cortex-M4) MCU and uses a GNU-based toolchain with openocd to flash and debug.

### 1. Install Build Tools (on Ubuntu or WSL)
```bash
sudo apt update
sudo apt install build-essential gcc-arm-none-eabi gdb-multiarch make openocd
```

### 2. Verify Installation
```bash
arm-none-eabi-gcc --version
openocd --version
```
### 3. Project Structure
```
firmware/
├── src/           # Application code and nessary files from external repo
├── config/        # HAL & board configuration TODO
├── lib/           # STM32CubeWL HAL + CMSIS
├── startup_stm32wle5xx.s
├── stm32wl.ld     # Linker script
└── Makefile
```

### 4. Build and Flash
#### Build firmware
make

#### Flash to STM32 (via ST-Link)
make flash

## Development Notes

This firmware is being developed for a LoRa-based data-acquisition system. Its main goals are to:
- Sample digital data from the ADC and other digital sensors over I2C interface
- Transmit the collected data through LoRa
- Store measurements in an external storage module through SPI interface
- Provide real-time debug output over UART
- Implement a low-power architecture using timer-driven interrupts

### HAL Module Configuration
The file `firmware/src/stm32wlxx_hal_conf.h` controls which HAL peripheral modules are enabled in the project.  
To disable a module, comment out its corresponding `HAL_xxx_MODULE_ENABLED` line.

### How to Use HAL Functions
To learn how a specific peripheral works, refer to the HAL driver source files located in:
`firmware\lib\STM32WLxx_HAL_Driver\Src`

### File Structure of Src
I think our initial goal for firmware/src files struture is to put all the header and source code under one folder, but to make clearer for each module we develop, I lay out our code strucute like:
```
src/
├── app/
│   ├── main.c
│   └── main.h
├── i2c/
│   ├── i2c_driver.c
│   └── i2c_driver.h
├── lora/
│   ├── lora_app.c
│   └── lora_app.h
├── power/
│   ├── power_mgmt.c
│   └── power_mgmt.h
├── scheduling/
│   ├── scheduler.c
│   └── scheduler.h
├── sensors/
│   ├── ads1110.c
│   ├── ads1110.h
│   ├── sensor_digital.c
│   └── sensor_digital.h
├── storage/
│   ├── data_buffer.c
│   └── data_buffer.h
├── uart/
│   ├── uart_debug.c
│   └── uart_debug.h
├── Other system configure files... (Placement TBD)
```

### LoRa module
The LoRa module was built using the reference project at
https://github.com/Seeed-Studio/LoRaWan-E5-Node/tree/main/Projects/Applications/LoRaWAN/LoRaWAN_End_Node  

It relies on the SubGHz LoRa driver that I included in `firmware\lib\SubGHz_Phy`. At first, I tried to use the radio driver on its own to configure LoRa directly on the chip, but it didn’t work until I pulled in most of the `.c` and `.h` files from the example application. Those files appear to contain essential HAL initialization and system-level setup that the driver depends on. Because of this, all files placed directly under the `src` folder are currently used for HAL and board-level configuration. After adding them, I was able to configure LoRa transmission successfully and confirm it using my SDR.

Some of these files directly under `/src` are clearly unnecessary and should eventually be removed. I will check after all system have been set up.

I also tried to understand the original Seeed code to get more understanding for lora, but it includes extensive LoRaWAN network-server configuration. This adds a large amount of logic beyond the PHY layer, which makes it harder to follow for our case for point-to-point LoRa use case. So I directly referred to `radio.h` under `firmware\lib\SubGHz_Phy` to locate the functions used for radio initialization and configuration. 

All of the LoRa application code is organized under `src/lora`.

### Scheduling Interrupt
The scheduling system of transmitting and sampling for now is implemented using a timer-based interrupt driven by TIM2 timer. I abandoned initial thought for implementing FreeRTOS for such structure. (Yes, a bit complex for me - I overestimate myself and maybe not too important to consider that if we want to add more color for the firmware, but at first we should have a at least functional firmware!)

The timer is configured to generate an interrupt every millisecond, and its internal counter is used to track elapsed seconds. Inside the interrupt callback, the firmware increments a `second_counter` and check if our counter has reached the setup sampling and transmission intervals. When the accumulated seconds meet the intervals goal, the interrupt will set `sample_flag` or `tx_flag`, while in the main loop will use to check to decide which action to take.

In the main loop, using `__WFI();` to let CPU just sleep while no interrupt.

All of the scheduling code is organized under `src/scheduling`.



