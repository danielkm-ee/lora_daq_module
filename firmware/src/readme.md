# Source Code Structure

## Modules

- `app/` - Main application
- `lora/` - LoRa communication
- `i2c/` - I2C sensors
- `uart/` - UART interface
- `power/` - Power management
- `scheduling/` - Task timing


## step up files (Don't Modify)
Files in `src/` are configuration/setup files required by our chip — do not edit them.

## Adding a Module

1. Create `src/your_module/`
2. Add `your_module.h` and `your_module.c`
3. Update Makefile
4. Test before integrating

## Build
```bash
make clean
make
make flash
```