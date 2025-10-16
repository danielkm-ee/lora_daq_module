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
