# Development Notes - ADC Integration
auther: saleh sabti

On October 16, 2025, I researched the ADC component for our LoRA DAQ system, focusing on the ADS1115 integrated with the Nordic nRF52840. Here’s my findings based on the datasheets I looked up:


### Technical Findings

**I2C Address Configuration:**
- ADDR pin connection determines I2C address 
- Four I2C addresses (0x48-0x4B) Default 0x48 when ADDR→GND 
- I2C comm. operates at standard (100kHz), fast (400kHz), or high-speed (3.4MHz).
- contains four 16-bit registers accessible via an 8-bit pointer register

**Config register bit mapping (default 0x8583):**
- Bit 15 (OS): Operational status 
  - 0: No effect (read), device performing conversion (write)
  - 1: Start single conversion (write), device not converting (read)
- Bit 8 (MODE): Operating mode
  - 0: Continuous conversion
  - 1: Single-shot (power-down after conversion)
- Important: ADDR pin must be tied to something, not floating!

**Power**
ADC Power Consumption:
- Continuous mode: 150µA @ 3.3V
- Single-shot mode: 0.5µA sleep (huge savings!)
So now I guess if our measurement cycle is (every 5 min):

**Noise and Filtering**
- Hardware: RC filter (1kΩ + 100nF) gives 1.6kHz cutoff between sensor output and ADC input.
- we can do it in software but we need a library

**Conversion timing analysis (sample per min.):**
- 8 SPS: 125ms per conversion
- 128 SPS: 7.8ms per conversion (default)
- 860 SPS: 1.16ms per conversion
  - **To save power we can use single shot mode (8SPS) where it sleep between readings. For continuous monitoring, using 128 SPS mode and update each channel reading at ~32ms intervals.**
 
**Noise and Filtering**
- RC filter (1kΩ + 100nF) gives 1.6kHz cutoff between sensor output and ADC input.
- we can do it in software but we need a library

### Resources & Links
**Datasheets**
- [ADS1115 Datasheet](https://www.ti.com/lit/ds/symlink/ads1115.pdf)
- [Ultra-Small, Low-Power, 16-Bit ADC with Internal Reference (Rev. B)](https://cdn-shop.adafruit.com/datasheets/ads1115.pdf)

**Code Libraries**
- [Adafruit ADS1X15 Library](https://github.com/adafruit/Adafruit_ADS1X15) - implementation


# October 26, 2025 - ADC Schematic Design

On October 26, 2025, I worked on creating the schematic for the ADC circuit, which includes the sensor input, low-pass RC filter, protection diodes, and the I2C interface connections. Here's what I did based on the datasheets I looked up:

### Implementation
**Design References:**
I started in the datasheet:
- Figure 7-1: Shows the ADS1115 block diagram with all components and I2C interface
- Figure 9-1: Typical connections for the ADS1115
- Figure 9-5: Basic hardware configuration

**Component Selection & Placement:**
- ADS1115 - ADC with I2C interface
- Sensor input connectors 
- Input protection diodes
- Input filter: RC low-pass filter (fc ≈ 3.4 kHz)
  - R1: 1k ohm series resistors
  - C1: 47nF capacitors
- R3, R4: 10k ohm I2C pull-up resistors
- C3: 0.1uF VDD decoupling capacitor

**Filter Design:**
- Hardware: RC filter (1kΩ + 47nF) gives 3.4kHz cutoff between sensor output and ADC input
  - To prevent high-frequency noise

**Next Steps:**
- Connect input signal path: sens. => Protection => R1 =>  C1 =>  AIN0
- Run ERC to check for errors

### Git Activity
```bash 
git checkout -b schematic-adc
git add hardware/lora_daq_module/adc.kicad_sch
git commit -m "added ADC input filter schematic"
git push origin schematic-adc
```

### Resources & Links
**Datasheets:**
- [ADS1115 Datasheet](https://www.ti.com/lit/ds/symlink/ads1115.pdf)

# Nov 10, 2025 - ADC Schematic Design Optimization

On November 10, 2025, I optimized the voltage divider resistor values after reviewing the OPA333 datasheet. Daniel suggested we could meet three requirements with one voltage divider instead of adding separate protection circuitry.

### Problem Identified
While reviewing **OPA333 datasheet Section 7.3.2** (Input Voltage), found critical requirement:
> "Input voltages that exceed the power supplies can cause excessive current to flow into or out of the input pins. Momentary voltages greater than the power supply can be tolerated if the input current is limited to 10 mA. This limitation is easily accomplished with an input resistor, as shown in Figure 18."

Since our soil moisture sensor outputs up to 5V (exceeds 3.3V supply by 1.7V), we need input current protection.

### Solution
Daniel suggested increasing the voltage divider ratio to accomplish three things simultaneously:

**1. Voltage divider:** Scale 5V → 4.096V for ADS1115 optimal range  
**2. Input protection:** Limit current to <10mA per OPA333 datasheet  
**3. FSR matching:** Stay within ±4.096V ADS1115 range for 125µV resolution

### Design Calculations

**Voltage divider:**
```
Target: 4.096V from 5V input
For voltage divider: Vout = Vin × (R2/(R3+R2))

R3 = 47kΩ  (series input protection)
R2 = 220kΩ (to ground)

Voltage scaling: 5V × (220k/(47k+220k)) = 4.12V 
Input protection: 5V/47kΩ = 106µA
FSR match: 4.12V < 4.3V max 
```

**RC filter cutoff:**
```
R1 = 1kΩ
C1 = 47nF (changed from 100nF)
f_cutoff = 1/(2π × 1k × 47nF) = 3.39 kHz
```

### Component Updates
- R3: 2.2kΩ → **47kΩ**
- R2: 3.9kΩ → **220kΩ**  
- C1: 100nF → **47nF**

**Schematic Status:** Finalized - only BOM and component boxing remaining

### ADS1115 Configuration Planning (next steps)
```
Gain: ±4.096V (GAIN_ONE) → 125µV resolution
Data rate: 128 SPS → 8ms conversion time
Mode: Single-shot → 0.5µA sleep current (vs 150µA continuous)
Resolution: 16-bit → 65,536 counts
Config register: 0xC383
```

### Wio-E5 I2C Interface
Started looking into STM32 development:
- **I2C2 pins:** PB15 (SCL), PA15 (SDA)
- **Tool:**  STM32CubeIDE for development
- **Debug:** ST-Link for programming interface

**Next Steps:**
- Complete STM32CubeIDE setup today
- Configure I2C2 peripheral
- Write I2C scanner test code

### Resources & Links
**Datasheets:**
- [OPA333 Datasheet](https://www.ti.com/lit/ds/symlink/opa333.pdf) 
- [ADS1115 Datasheet](https://www.ti.com/lit/ds/symlink/ads1115.pdf)
- [Wio-E5 Module Datasheet V1.1](https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20module%20datasheet_V1.1.pdf)