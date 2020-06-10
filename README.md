# PS2ToJoystick
PS2 Dualshock 2 PAD to PC Joystick

# Table Of Contents

1. [Project Features](#project-features)
2. [Part List](#part-list)
3. [Electronics](#electronics)
    1. [Wiring](#wiring)
4. [Software](#software)
    
# Project Features

# Part List

# Electronics

## Wiring


Female Socket (the opposite to DualShock 2 connector) (looking from FRONT)
```
\-------------------------/
 \ 1 2 3   4 5 6   7 8 9 /
  \---------------------/
```

|Dualshock PIN   |color   |function |                      arduino PIN|
|----------------|--------|---------|--------------------------------:|
|1               |brown   |DATA     | (with pullup resistor 4.7KOhm) 6|    
|2               |orange  |DATA     |                                7|    
|3               |grey    |VIBRATION|                                 |    
|4               |black   |GROUND   |                              GND|    
|5               |red     |VCC      |                              VCC|    
|6               |yellow  |SELECT   |                                8|    
|7               |blue    |CLOCK    |                                9|    
|8               |white   |UNKNOWN  |                                 |    
|9               |green   |ACK    |                                   |    

Pull Up Resistor wiring diagram

```
                                ARDUINO PIN 6
                                    |
                                    |
 VCC -------------[ R 4.7KOhm ]-----+
                                    |
                                    |
                                DUALSHOCK 2 PIN 1 (BROWN)
```
# Software



# Resources

https://www.instructables.com/id/PS2-Wire-Controller-and-Arduino-control-LEDs/

http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/
https://github.com/madsci1016/Arduino-PS2X/tree/master/PS2X_lib
https://store.curiousinventor.com/guides/PS2

https://github.com/MHeironimus/ArduinoJoystickLibrary
https://github.com/madsci1016/Arduino-PS2X

ATmega32U4 -> Board: Leonardo.
