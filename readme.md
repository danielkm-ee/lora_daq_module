# LoRA DAQ Node
For our Electrical and Computer Engineering practicum at Portland State for 2025 we have developed a LoRA based data acquisition node that can be used for data collection and monitoring systems over large areas.

![](./docs/lora_daq_module_final_top.png)

This board is powered by the Seeed Studios Wio-E5 LoRA module for use with it's designated antenna.

## Supported Peripherals
We've done our best to provide a versatile range of peripherals for high quality data collection and application flexibility. This includes a UART interface for communicating with the device via it's USB-C port, a SWD interface for loading and debugging applications, a 16-bit ADC and I2C connector that can be used with analog and I2C compatible sensors.

The device may be powered via it's USB-C (USB 2.0) connection, or by a 1s1p lithium ion battery. The device also
supports battery charging when powered by USB-C with the battery connected.

List of Peripherals
------------------
* 16-bit ADC
* I2C interface
* MicroSD card slot
* 2 external UART interfaces (one via USB-C port)
* 6-pin SWD header for ST-Link v2
* SMA connector for the 868-916MHz LoRA Antenna
* 2-pin JST EH connector for battery power and charging

## Credits
* Manaf Alali - hardware schematics
* Eyad Ahmed  - firmware
* Daniel Monahan - hardware schematics and layout
* Saleh Sabti - hardware schematics
* Dingyu Zhou - firmware

> All team members contributed to every aspect of the project, going beyond the general roles specified here.

> > *Portland State University ECE411 Practicum project, Fall 2025*
