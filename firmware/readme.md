# LoRA DAQ Firmware

## Toolchain Setup

This project targets the **STM32WLE5** (Cortex-M4) MCU and uses a GNU-based toolchain with openocd to flash and debug.

### 1. Install Build Tools (on Ubuntu or WSL)
```bash
sudo apt update
sudo apt install build-essential gcc-arm-none-eabi gdb-multiarch make openocd
sudo apt install -y clang-format
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
```
make
```
#### Flash to STM32 (via ST-Link)
```
make flash
```

#### External make
```
make tx         # Build firmware with TX configuration
make rx         # Build firmware with RX configuration
make flash_tx   # Flash TX firmware to the board
make flash_rx   # Flash RX firmware to the board
```


### 5. Every time before push
#### Run a clean build
```
make clean
make
```
#### Auto-format your C code
```
make format
```
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
I think our initial idea for the `firmware/src` files struture is to put all the header and source code under one folder, but to make clearer for each module we develop, I lay out our code structure as follows:
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

#### TODO:
- [x] cleanup structure for contribution and reviews
- [] cleanup the external relied system configuration files
- [] better code structure

### LoRa module
The LoRa module was built using the reference project at
https://github.com/Seeed-Studio/LoRaWan-E5-Node/tree/main/Projects/Applications/LoRaWAN/LoRaWAN_End_Node  

It relies on the SubGHz LoRa driver that I included in `firmware\lib\SubGHz_Phy`. At first, I tried to use the radio driver on its own to configure LoRa directly on the chip, but it didn’t work until I pulled in most of the `.c` and `.h` files from the example application. Those files appear to contain essential HAL initialization and system-level setup that the driver depends on. Because of this, all files placed directly under the `src` folder are currently used for HAL and board-level configuration. After adding them, I was able to configure LoRa transmission successfully and confirm it using my SDR.

Some of these files directly under `/src` are clearly unnecessary and should eventually be removed. I will check after all system have been set up.

I also tried to understand the original Seeed code to get more understanding for lora, but it includes extensive LoRaWAN network-server configuration. This adds a large amount of logic beyond the PHY layer, which makes it harder to follow for our case for point-to-point LoRa use case. So I directly referred to `radio.h` under `firmware\lib\SubGHz_Phy` to locate the functions used for radio initialization and configuration. 

Update: I have test with external dev board flashing RX firmware `make rx` and verify through a point-to-point test that our LoRa module is correctly configured and can transmit the correct packet signal that can be decoded.

All of the LoRa application code is organized under `src/lora`.

#### TODO:
- [x] Test the signal physically transmitted
- [x] Test the signal can be decode from other LoRa board


### Scheduling Interrupt
The scheduling system of transmitting and sampling for now is implemented using a timer-based interrupt driven by TIM2 timer. I abandoned initial thought for implementing FreeRTOS for such structure. (Yes, a bit complex for me - I overestimate myself and maybe not too important to consider that if we want to add more color for the firmware, but at first we should have a at least functional firmware!)

The timer is configured to generate an interrupt every millisecond, and its internal counter is used to track elapsed seconds. Inside the interrupt callback, the firmware increments a `second_counter` and check if our counter has reached the setup sampling and transmission intervals. When the accumulated seconds meet the intervals goal, the interrupt will set `sample_flag` or `tx_flag`, while in the main loop will use to check to decide which action to take.

In the main loop, using `__WFI();` to let CPU just sleep while no interrupt.

All of the scheduling code is organized under `src/scheduling`.

#### TODO:
- [x] Think of a way for low power schdeduling
- [] Power profiling to see if the codebase really enable low power

### UART Debug
The current stage of UART is enable upon code that I took from the reference project at
https://github.com/Seeed-Studio/LoRaWan-E5-Node/tree/main/Projects/Applications/LoRaWAN/LoRaWAN_End_Node

The UART setup is handled through functions in `firmware\src\timer_if.c`(taken from example code), which has enabled the UART peripheral. Using this setup, I wrote application UART codes for serial debugging outputs.

All of the UART code is organized under `src/uart`.

#### TODO:
- [x] Enable UART to debug
- [] Using UART to see the data sampling and receiving

### Sensor interface and Sensor Data

#### TODO:
- [] Enable I2C
- [] Data structure for nessary information and transmission 
- [] Configure sensors and ADC properly
- [] Validate correct data acquisition

### Test
I want all the test code that everyone commits to be formatted using clang-format, and I also want testing to be driven through the Makefile.

#### TODO:
- [] enable CI test
- [] think of unit test cases



## Stage of Development
TODO:
- [x] Building and Flashing code structure
- [x] Enabling LoRa
- [] Enabling I2C
- [] Configure ADC and digital sensor if needed
- [x] UART debuging
- [] Github CI test
- [] LoRa testing
- [] Sensor data validation

### (HEAD -> firmware_init, origin/firmware_init)

#### Stage commit 398cc2e363845a623673a12c9f33951d18dbbad1 
At this stage of the commit, we are now able to enable LoRa transmission, UART debugging output, and schedule the main loop for sampling and transmitting using a timer-based interrupt. After you build the project with the Makefile and flash the firmware, you should see UART output over USB like:
```
Sampling...
Transmitting...
Transmission complete.
```
This repeats every 1 second. If you also connect the SMA output to a signal analyzer or an SDR, you should see the waveform dancing at 915 MHz.
#### Stage commit 2383b4ef38c4a43d3942959597c4a2f3eaec716b
At this stage of the commit, we have initialized the bare-metal coding structure so the firmware can be built and flashed without relying on any external tedious GUI tools. Instead, we build the code using gcc and flash it using OpenOCD. In this commit, we managed to drive the onboard LED through GPIOB PIN 5 using the official HAL driver.