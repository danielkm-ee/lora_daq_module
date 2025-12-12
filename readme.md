# LoRA DAQ Module

ECE411 Capstone project

### connect and read UART 
sudo picocom -b 115200 /dev/ttyUSB0
sudo picocom -b 115200 /dev/ttyUSB1

If using WSL:
usbipd list

usbipd bind --busid 9-2

usbipd attach --wsl --busid 9-2