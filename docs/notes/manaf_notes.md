# Development Notes
Author: Manaf Alali 

# Power Management Module Notes 10/21
- Started mapping out the full power system for the board.
- Goal: The board must be powered by a 3.7V LiPo battery and be rechargeable via USB.
- The system needs to provide a stable 3.3V for the main LoRa MCU, ADC, and other chips.

# November 8, 2025 - Design Change (Rev 2)
- Action: Removed the 3.3V buck-boost converter from the original design.
- Reason: We decided to simplify the power tree. Instead of running two separate 
buck-boost converters from the battery, we are now using a two-stage approach.

# New Design:
- A 5V buck-boost (TPS63002) creates a stable +5V rail from the battery.
- I then added a simple Low-Dropout Regulator (LDO), the MCP1700, which
takes that stable 5V as its input and efficiently steps it down to our main +3.3V rail.