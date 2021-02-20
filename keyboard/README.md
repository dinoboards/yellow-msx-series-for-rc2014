# MSX Keyboard for RC2014

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

The MSX keyboard, is a full size MSX compatible keyboard design for RC2014 system with the PPI interface board

The keyboard connects to the PPI, via a 16 way IDC connection.

Key Features:

* Unambiguous key detection (diodes associated with every switch)
* Power, caps-lock, and code indicator LEDs
* Cherry 5 pin switches
* 73 keys

> Please note that this keyboard requires the PPI board for operation


## Bill of Materials

|Count   | Name                 |  Designator |
|:------:|-------------------------|-------------|
| 2	     | 0.1uF               | C1,C2 |
| 81     | 1N4148	             | D1-D81 |
| 3      | 3mm LEDS            |
| 1      | IDC Socket          | P1 |
| 3      | 470 Ω               | R2,R3,R1 |
| 1      | 10k Ω x 8           | R4 |
| 73     | switches            | SW1-SW73 |
| 2      | 74HC138             | U2,U1 |
| 4	     | 2U key stabliser    |
| 1      | 6.25U key stabliser |
| 1	     | keycaps set         |
| 1	     | PCB                 |

## Sample Apps

The msxkeyb.com application can be used to test the keyboard matrix lines are working correctly.

## Using keyboard on RomWBW

Although the keyboard and PPI board are designed to work under MSX software, it can be used in a stock RC2014 build.

You can write your own software to interface to the keyboard (see the demo app msxkeyb), or you can activate the
custom driver written for RomWBW's HBIOS.  The drivers code is on my fork of [RomWBW](https://github.com/vipoo/RomWBW/tree/dean/dev-v9958).

(Planning to submit this driver to the main project)

## Resources

* Schematic: [schematic.pdf](./schematic.pdf "Schematic")
