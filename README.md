This is an attempt to make a PCB version of [Hagiwo's Clock Multi Divide module](https://note.com/solder_state/n/n8907f2f6e8f5).

# Notes

- I'm using schematic symbol for Arduino Nano v3.x, check that this matches my actual Nano

## Diodes and voltage protection

Which diodes protect against which things? Original text says that the circuit:
1. Does not break due to overvoltage
2. Does not break due to negative voltage
3. Does not break due to overcurrent
4. External clock does not enter the internal clock output
5. Internal clock output does not enter the external clock input
But which diodes do which things?

These diodes are there for protecting the arduino if you plug something in that sends voltages it can’t handle.
Everything negative will go to ground thru one set of diodes, everything over 5V will go to the 5V rail thru the other set.
Out has only one diode because there is already one on pin D13.

## Power

- Arduino is given VIN of 12V? Or is it somehow powered from the 5V rail?
- I notice Higawo uses a 16 pin connector
- But Arduino Nano operating range is quoted as 7-12V
- Should we connect capacitors to the +-12V power connector rails?

## Updating the schematic

There were some small issues with the original Hagiwo schematic. I made the following changes in my KiCad schematic:
- in the External Clock In section, there should be a dot connecting two vertical BAT43 diodes
According to the discussion [here](https://lookmumnocomputer.discourse.group/t/arduino-clock-multiple-divider/2612/3), for the Master Clock Out section:
- The initial diode after Arduino pin D13 should be a small signal diode (IN4148 or similiar)
- The SchottKey diode following the 470 Ohm resistor should actually come before the resistor

## PCB Issues

- My SPDT switch is not board mountable and will need to be wired to the board instead. So I used the footprint "TerminalBlock:TerminalBlock_bornier-3_P5.08mm"
- The Arduino nano will be mounted to the front of the PCB using pin headers
