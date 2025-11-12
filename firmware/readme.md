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
firmware/
├── src/           # Application code
├── config/        # HAL & board configuration
├── lib/           # STM32CubeWL HAL + CMSIS
├── startup_stm32wle5xx.s
├── stm32wl.ld     # Linker script
└── Makefile

### 4. Build and Flash
#### Build firmware
make

#### Flash to STM32 (via ST-Link)
openocd -f interface/stlink.cfg -f target/stm32wlx.cfg \
  -c "init; halt" \
  -c "program build/firmware.elf verify reset exit"