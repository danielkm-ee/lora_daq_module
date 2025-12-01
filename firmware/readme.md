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

### HAL Module Configuration
The file `firmware/src/stm32wlxx_hal_conf.h` controls which HAL peripheral modules are enabled in the project.  
To disable a module, comment out its corresponding `HAL_xxx_MODULE_ENABLED` line.

### How to Use HAL Functions
To learn how a specific peripheral works, refer to the HAL driver source files located in:
`firmware\lib\STM32WLxx_HAL_Driver\Src`