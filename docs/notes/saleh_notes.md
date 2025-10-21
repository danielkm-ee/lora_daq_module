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
