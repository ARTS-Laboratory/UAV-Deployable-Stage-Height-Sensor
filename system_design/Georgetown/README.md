# Georgetown
UAV deployable sensor package for measuring water height. This document lists the updates made to the sensor pacakge over time.
## V0.1
1. Oldest version
2. Basic datalogger setup with modular RTC and SD card
3. Accelerometer integrated into daughter board

## V0.2
1. Teensy 4.0 microcontroller
2. Added environmental sensor

## V0.3
1. Teensy 4.0 or Arduino Nano microcontroller
2. Added DC current sensor

## V0.4
1. Arduino Nano microcontroller
2. Magnet power control circuit on daughter board
3. No accelerometer on daughter board
4. Solar panel load sharing on daughter board

## V0.5
1. Major CAD and PCB update from V0.4
2. Microcontroller : Arduino Nano
3. Solar panels moved to the bottom of the sensor package
4. BMS added to manage solar load sharing in main PCB, daughter PCB eliminated
5. Uses dynamic sleep interval depending on battery voltage level
6. Uses permanent magnet to attach to steel structures
7. Easy sliding water-proof clear tube

## V0.5.1
1. Minor design updates in PCB from V0.5.

## V0.5.2 (present version)
1. Major power electronics update from V0.5.1
2. Uses Mosfet based switching to turn off device during sleep. Wakes up using RTC interrupt. Resulting in very low power consumption and longer deployment time.
3. Sensing features: Water level distance from sensor, Humidity, temperature, pressure, battery voltage (recorded against RTC time)

## V0.6 (Proposed)
1.  Back to being UAV deployable using [this system](https://github.com/ARTS-Laboratory/UAV-Understructure-Sensor-Deployment-and-Recovery-System).
2. Uses module that has [single ultra sound transducer](https://cdn.shopify.com/s/files/1/0550/8091/0899/files/11832.pdf?v=1726578772) to lower current consumption.
























