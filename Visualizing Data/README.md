# LCD Overview
The only board we have that has a LCD display built into it is the MSP430FR6989. We are able
to display characters to the display through an LCDDriver.c, LCDDriver.h, and stdlib.h files.
Using the 12 bit ADC we convert the analog signal from our photoresistor to a digital signal that
we display on the LCD. Our ADC is 12 bit so we have 4096 resolution bits. When the light is on
we should have a higher value closer to 4096 display on the LCD, when the photoresistor is
covered we should get a value closer to 0000.

## Hardware Configurations
We used the same configuration as in the sensors and signal conditioning part of the lab, using
the photoresistor.

## Photoresistor
A photoresistor is a light dependent resistor, in other words, it is a light controlled variable
resistor. As incident light intensity intensifies, the resistance of the photoresistor decreases. This
means that as incident light intensifies, the recorded output voltage should be higher. Below is a
schematic of the built circuit.

 ![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-jordan-pippen/blob/master/Pics%20%3B\)/Phototransistor%20schematic.jpg)

 ![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-jordan-pippen/blob/master/Pics%20%3B)/photoresistor%20circuit.JPG)
The 4.7k resistor is placed there to act as a voltage divider so that, even when the resistance of
the photoresistor is low, the entire 3.3V from the source is not present at Vout.

### Light On:
 ![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-jordan-pippen/blob/master/Pics%20%3B\)/LCD/LIGHT_ON.JPG)
### Light Off:
 ![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-jordan-pippen/blob/master/Pics%20%3B)/LCD/LIGHT_OFF.JPG)
When the light is covered the output voltage drops from 3139 to 1338. This shows that the
phototransistor circuit is working properly and the output voltage is successfully being sent and
read across the LCD Display.

## How to Implement Code
1. Gather photoresitor.
2. Construct the photoresistor circuit using schematics as a reference.
3. Copy main.c code and copy it into Code Composer.
4. Debug and program your MSP430FR6989.
5. Connect all wires from MSP430F5529 to circuit. (3.3V to power rail, GND to ground
rail, P1.3 to Vout of circuit being tested).
6. Run the code on your MSP430FR6989