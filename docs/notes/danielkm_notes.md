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


# Nov 12th -- Schematic updates and Preliminary layout

## Schematic updates 
Yesterday I was able to merge the drawing sheets for the adc, lora module, and power management schematics. I worked on the schematic to get it ready for layout, and added to the power management schematic.

Here's a quick changelog:
--------------
* Added power switching IC to select USB vs Battery power to the power management schematic
* Added two pin connector for battery
* added pullup for the reset pin on the lora module
* added large DNP capacitor at the 3V3 output from the CP2102 as a fallback power supply
* added footprints to all components

More detailed comments
----------------
There are two ways we can arrange the power circuits. We could (1) boost the 3.7V battery voltage, then perform power switching between VBUS and VBATT, then send the winner to the LDO. or (2) compare VBUS and VBATT, then send the winner to the boost/buck converter, then the LDO. Currently I'm using option (1), but we may want to move to option (2) as the power switching circuit will help to limit current draw from the battery when we are not using it.

When adding footprints I attempted to be accurate as I could without making custom footprints. It's possible that the footprints I've used need modifications and we'll have to check on that before we order the boards next week.

## Preliminary Layout
Tonight I was able to arrange the components, and I've used reference layouts for every aspect of the circuit. This includes the layouts provided on the data sheets for the MCP73833, TPS2116, and TPS63002, and otherwise I used the dev board and application notes as a reference.

I was not able to lay out all of the traces, particularly because I haven't decided how I want my power planes set up. Tonight I focused on fitting the components onto the board with a great attention to detail on their arrangement and orientations. I'll be submitting the preliminary layout and current schematics to canvas for the week 6 assignment, and to github (for the assignment and for our own reference.)


# Nov 14th - BOM and Rough draft of CAD
I'm currently working through the symbol fields table component-by-component to assign part numbers and finalize our footprints as best as I can. I've run into a few cases where we may need to make some changes and I want to be sure to explain them here!

changelog:
----------
* Replacing the ADS1115 with the ADS1110
> Basically, the ADS1115 comes in three packages; a QFN, a TSSOP, and a VSSOP. When assigning the prelim footprint I used the TSSOP because it is much more friendly for hand soldering, with a pin spacing of about .85mm. It's closest counterpart would be the VSSOP, though this has a .5mm spacing which will make it very difficult to solder. The TSSOP would have been fine to use, but Digikey only stocks the VSSOP and the QFN, which present considerable challenges to an already difficult buildup of this board thus far. 
> Because we aren't using the four channels on the ADS1115, there is no need to stick to this specific part. Additionally, the added channels create a more expensive and difficult ADC circuit for our board which is hard to justify for this prototype. I looked to the ADS1119 which comes in a TSSOP with the same number of channels, but an even more reasonable option here is the ADS1110 which comes in an SOT23-5 and has a single differential input. Digikey has around 5k stocked and they come in multiple variants with different I2C addresses, and the SOT23-5 is a much more hand-solder friendly package. This will require some careful review of the ADC circuit, though the ADS111x series does a good job of being close to standard so we may be able to avoid major changes here.

When looking for resistors I noticed a vast majority of 10kΩ resistors, I assume these are for pull ups. I've been using 4.7k for these, which is a very viable option, but I wonder if we should move to 10k to reduce quiescent current draw as were operating on battery power... will look into this later on.

Another note, I found that the CP2102N has some form of battery charger detection that I can't believe I've missed up until now. Not sure what this entails but it's another thing to explore as I review our power schematic.

As far as resistors go, I am using 0603 1/10W everywhere for now. Once we've had another pass over everything we can bump up footprints and power tolerances as needed.

> Also, note to self.... FILL OUT THE PRICE PER UNIT FIELD AS YOU GO!!  
> Currently 75 percent through the part assignment and will have to go over the whole list again T.T

> > *Actually, I think digikey will be able to give me pricing based on a csv.. so crisis averted.

For the 3.3uH inductor used on the buck/boost tps63002 I am using the Coilcraft LPS3015 series based on the reccommended inductor list from table 2 on page 11 of the IC's datasheet. This is a marketplace item, shipped through coildcraft directly. In general I have avoided marketplace items.

Another exception I've made here is the resistor rule mentioned earlier, I am using an automotive grade 50Ω resistor at the pi network at the antenna, though this may be overkill. For refrence, the price difference is about 6x, where typical resistors are 10 cents, this one is 62 cents. like i said,,, maybe overkill!

Another area we'll definitely want to explore is connectors, particularly headers. Currently I'm ordering pre-cut pin headers in the array sizes we need, but since we're hand-soldering these down that's completely unnecessary. May be cheaper to just order a pair of long 1-row and 2-row headers to cut down to size by ourselves.

# antenna layout
I've looked into the microscrip design for the antenna connector, had some discussion with Maxwell, and consulted online sources and came up with the following options:

Using the microstrip line calculator on KiCAD with the following parameters:
| er | tan(delta) | rho   | H     | H(top) | T   | ur | freq  | L  | Z0 |
|----|------------|-------|-------|--------|-----|----|-------|----|----|
|3.67| 0.012      |1.72e-8|.2021mm| .2021mm|.0432| 1  | 880MHz|10mm|50Ω |

I got W = 0.486816 which I rounded to 0.487 for the microstrip trace width.

We have two options, either trust the microstrip working, or adding a pi network for impedance matching if we run into issues.

## Keeping the pi network
to keep the pi network, we will have to use 0402 pads and place them directly in line with the microstrip. They have to be as close to the 0.487 trace width as possible, or they present an impedance discontinuity.

This complicates things in general, so the other option is...

## Ditch the pi network
Just lay out the microstrip and bet things will work out okay. Worst case we can cut open some traces and try our best to do something about an impedance mismatch. Based on timeline, this may be our best option.


# Ground gap and via fencing
Back to the general layout decisions, I've used the convention that the ground gap should be H1+H2, which for our board is 0.4042mm, and our via fencing should be spaced no more than (1/10)λ = (1/10)(1/(2πf))*(3e8) = 0.005425m or 6mm.
I've applied both of these rules to the antenna layout.

# Final design notes, changes, checkoffs
I've added 1812 resistors at the inputs and outputs of the power circuits. One from VBUS to the MCP7338, another from the battery to the buck/boost, and a final one at the input of VBUS to the power mux. 
Final failsafes for power will be to remove the 0Ω to the power mux, then mod wire the VBUS pad to the LDO. We would also want to desolder the power mux I believe.

I've fixed the LEDs for the battery charger, originally I made up their indicators but now we have a green led at STAT2 which indicates full/stopped charging. Amber at stat1 indicates charging in progress, and red at PG indicates power is on/off.

I added a 3 pin JST connector for the i2c bus and removed the testpoints for the sda and scl to save room.

I added the pi network back in and set up the board for a 1.6mm 4 layer jlcpcb board with 1oz top and 10z middle layers.

I also replaced the two pin header used for the battery with a two pin jst receptacle.

I labeled all test points, shrunk reference texts to declutter the board, and made necessary modifications to flood planes and traces to facilitate power jumpers.

I added a reset switch to restart the microprocessor that can be left unsoldered if desired.

The ADC input filter has been rearranged and redesigned according to andrews instructions,
the buffer has been moved to the input, the voltage divider was removed and the power rails were changed to +3.3V. I put the anti-aliasing low-pass filter at the output of the buffer and used a 47Ω and 4.7uF to set the cutoff at 720Hz. I used the 47Ω with the aim of having a low output impedance.

I updated the antenna trace width to 0.49mm according to the updated jlc pcb parameters, I then added the pi impedance matching network using 0402 components. They are a bit large, though I believe we may be close enough here and wanted to bring the option back in incase the jlc impedance calculations aren't great.


