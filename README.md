# SPI EEPROM Access Library for RaspberryPi based on spidev_lib

Started from the nopnop2002 / Raspberry-spi-eeprom project, but that used one the wiringPi lib which no longer exist.
This now uses spidev_lib and contains those files so it has no dependencies that might disappear in some dark future.
And the code has been cleaned up a bit with error handling, timeouts, ... 
I only used it with an M95640.
Use at your own risk, I just needed something quick to read and write that eeprom. 

---

# Memory size

## ST Micro   
There are several variations in the M95 series.   
4.5 V to 5.5 V for M95xxx   
2.5 V to 5.5 V for M95xxx-W   
1.8 V to 5.5 V for M95xxx-R   

__M95xxx don't work with Raspberry Pi__

|Device|# of Bits|# of Bytes|Byte Address range|Page Size(Byte)|Page Address Range|
|:---|:---|:---|:---|:---|:---|
|M95080|8K|1024|0x00-0x3FF|32|0-31|
|M95160|16K|2048|0x00-0x7FF|32|0-63|
|M95320|32K|4096|0x00-0xFFF|32|0-127|
|M95640|64K|8192|0x00-0x1FFF|32|0-255|
|M95128|128K|16384|0x00-0x3FFF|64|0-255|
|M95256|256K|32768|0x00-0x7FFF|64|0-511|

## ATMEL   

|Device|# of Bits|# of Bytes|Byte Address Range|Page Size(Byte)|Page Address Range|
|:---|:---|:---|:---|:---|:---|
|AT25010|1K|128|0-0x7F|8|0-15|
|AT25020|2K|256|0-0xFF|8|0-31|
|AT25040|4K|512|0-0x1FF|8|0-63|
|AT25080|8K|1024|0-0x3FF|32|0-31|
|AT25160|16K|2048|0-0x7FF|32|0-63|
|AT25320|32K|4096|0-0xFFF|32|0-127|
|AT25640|64K|8192|0-0x1FFF|32|0-255|
|AT25128|128K|16384|0-0x3FFF|64|0-255|
|AT25256|256K|32768|0-0x7FFF|64|0-511|
|AT25512|512K|65536|0-0xFFFF|128|0-511|

---

# Build

## ST Micro   
for M95080   
cc -o main main main.c eeprom.c spidev_lib.c -DM95080

for M95160   
cc -o main main main.c eeprom.c spidev_lib.c -DM95160

for M95320   
cc -o main main main.c eeprom.c spidev_lib.c -DM95320

for M95640   
cc -o main main main.c eeprom.c spidev_lib.c -DM95640

for M95128   
cc -o main main main.c eeprom.c spidev_lib.c -DM95128

for M95256   
cc -o main main main.c eeprom.c spidev_lib.c -DM95256

## ATMEL   
for AT25010   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25010

for AT25020   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25020

for AT25040   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25040

for AT25080   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25080

for AT25160   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25160

for AT25320   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25320

for AT25640   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25640

for AT25128   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25128

for AT25256   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25256

for AT25512   
cc -o main main main.c eeprom.c spidev_lib.c -DAT25512


---

# Usage

```
Active spi 0 with 1 chip select line on pin 8 (for example)
sudo dtoverlay spi0-1cs cs0_pin=8 

To read the contents of a connected eeprom and write it to an output file contents.bin
./main -o contents.bin 

To write the contents to a connected EEPROM from an input file contents.bin
./main -i contents.bin 

```
