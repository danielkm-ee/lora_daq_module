# Development notes
author: daniel

## RN2483 using nRF52810 host
I’ve looked into two LoRA solutions tonight (10/9/25) and this is the one I think we would enjoy the most.

I read the datasheets for the STM32wl55cc with the RN2483, the stm board uses cortex m4 and cortex m0 cpus, the m4 is for general embedded systems use, the m0 is the rf/secure/lora enabled cpu. It gets complicated.

I like the RN2483 because it’s controlled through a UART with acii commands and has a nice LoRAWAN protocol api that we could control using any MCU we choose. This allows us the freedom to use nRF as we wanted. The caveat compared to using the stm board is that st provides it’s own debug programmer, and nrf requires some for of SWD like J-Link. Initally I was like.. damn.. because J-Flash probes are like $400-$1200, but then I found the J-Link EDU Mini for $60.

We’d need an nRF chip supporting uart, swd, and i2c at the minimum, so I found the cheapest option and it’s the nRF52810, for about $2.40 per chip in a QFN48 or QFN32 package.

[https://shop-us.segger.com/product/j-link-edu-mini-8-08-91](https://shop-us.segger.com/product/j-link-edu-mini-8-08-91)
When we eventually need to flash a bootloader on the nRF chip, this is the most affordable option.

#### Useful sources from tonight
[lorawan primer](https://www.iotforall.com/a-primer-for-loralorawan)
[antenna strip trace impedence calculator for future layout stuff](https://www.pasternack.com/t-calculator-stripline.aspx?srsltid=AfmBOoq0oVWRDSZBU8QKXRaA19tC9YBYH8PjxBghffFQ5Ye9wR9uHJyM)
[another lorawan primer](https://en.iotrouter.com/in-what-topology-are-lora-and-lorawan-network-structures-deployed/)


# Wio-E5 Schematic development notes

Making a note here while I try to finish the schematic for the lora module.

During the design review Eric brought up some good points regarding the peripherals. Here's what we've landed on thus far...

On-Board Peripherals:
- I2C EEPROM - for storing device configuration, calibration parameters, unique device ID and whatnot
- SD Card - for storing data locally maybe used for debugging or as a failsafe, but we figured it would provide some much-obliged flexibility.

Other things I've been working to implement are the SWD connections. According to docs from STM32 and the datasheet for the wio-e5 board the default pins for the serial peripheral interface (SPI) SWD/ST-Link are as shown in the table here.

| Wio Package Pin | STM32 Pin Name | Description |
| :-------------: | :------------: | :---------- |
|        3        |      PA13      | SWDIO       |
|        4        |      PA14      | SWCLK       |

As far as the remaining pins, I've attempted to retain the pinout according the Wio-E5 datasheet, though I am providing the exported pinout in [docs](../wio-e5pinout.csv).

Here's a table showing the current pinout:

| Position | Name           | Type   | Signal           | Label |
| -------- | -------------- | ------ | ---------------- | ----- |
| A1       | VSSSMPS        | Power  |                  |       |
| A2       | VDDSMPS        | Power  |                  |       |
| A4       | PA14           | I/O    | DEBUG_JTCK-SWCLK |       |
| A5       | VDDA           | Power  |                  |       |
| A7       | VDD            | Power  |                  |       |
| A8       | VBAT           | Power  |                  |       |
| A9       | PA12           | I/O    |                  |       |
| B1       | VLXSMPS        | Power  |                  |       |
| B2       | VFBSMPS        | Power  |                  |       |
| B3       | PA15           | I/O    | I2C2_SDA         |       |
| B4       | PB15           | I/O    | I2C2_SCL         |       |
| B5       | VREF+          | MonoIO |                  |       |
| B6       | PC14-OSC32_IN  | I/O    |                  |       |
| B7       | VSS            | Power  |                  |       |
| B8       | PA13           | I/O    | DEBUG_JTMS-SWDIO |       |
| B9       | PA11           | I/O    |                  |       |
| C1       | PB3            | Output | GPIO_Output      |       |
| C2       | PB4            | Output | GPIO_Output      |       |
| C3       | PB7            | I/O    | USART1_RX        |       |
| C4       | PB9            | I/O    | SPI2_NSS         |       |
| C5       | PC15-OSC32_OUT | I/O    |                  |       |
| C6       | PB14           | I/O    | SPI2_MISO        |       |
| C7       | PC13           | I/O    |                  |       |
| C8       | PA10           | I/O    | SPI2_MOSI        |       |
| D2       | PB5            | Output | GPIO_Output      |       |
| D3       | PB8            | I/O    |                  |       |
| D4       | PC2            | I/O    |                  |       |
| D5       | PC3            | I/O    |                  |       |
| D6       | PA0            | I/O    |                  |       |
| D7       | PB13           | I/O    | SPI2_SCK         |       |
| D8       | PB2            | I/O    |                  |       |
| D9       | VSS            | Power  |                  |       |
| E1       | PB6            | I/O    | USART1_TX        |       |
| E2       | VDD            | Power  |                  |       |
| E3       | VSS            | Power  |                  |       |
| E4       | PC5            | I/O    |                  |       |
| E5       | PA9            | Output | GPIO_Output      |       |
| E6       | PB12           | I/O    |                  |       |
| E7       | PB1            | I/O    |                  |       |
| E8       | VDDRF          | Power  |                  |       |
| E9       | VDD            | Power  |                  |       |
| F1       | PC1            | I/O    | LPUART1_TX       |       |
| F2       | PC0            | I/O    | LPUART1_RX       |       |
| F3       | PC4            | I/O    |                  |       |
| F4       | PA6            | I/O    |                  |       |
| F5       | NRST           | Reset  |                  |       |
| F6       | PB0-VDD_TCXO   | I/O    |                  |       |
| F7       | VDDRF1V55      | Power  |                  |       |
| F8       | OSC_OUT        | MonoIO |                  |       |
| G2       | PC6            | I/O    |                  |       |
| G3       | PA1            | I/O    |                  |       |
| G4       | PB11           | I/O    |                  |       |
| G5       | VSS            | Power  |                  |       |
| G6       | VSSRF          | Power  |                  |       |
| G7       | VSSRF          | Power  |                  |       |
| G8       | VSSRF          | Power  |                  |       |
| G9       | OSC_IN         | MonoIO |                  |       |
| H1       | PA3            | I/O    | USART2_RX        |       |
| H2       | PA2            | I/O    | USART2_TX        |       |
| H3       | PA7            | I/O    |                  |       |
| H4       | PB10           | Output | GPIO_Output      |       |
| H5       | VDD            | Power  |                  |       |
| H6       | VSSRF          | Power  |                  |       |
| H7       | RFI_N          | MonoIO |                  |       |
| H8       | VDDPA          | Power  |                  |       |
| H9       | VR_PA          | Power  |                  |       |
| J1       | PA4            | I/O    |                  |       |
| J2       | PA5            | I/O    |                  |       |
| J3       | PA8            | I/O    |                  |       |
| J5       | PH3-BOOT0      | I/O    |                  |       |
| J6       | RFI_P          | MonoIO |                  |       |
| J8       | RFO_LP         | MonoIO |                  |       |
| J9       | RFO_HP         | MonoIO |                  |       |

