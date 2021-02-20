# PPI Board Designed for RC2014

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

The PPI 82C55 board, designed for RC2014 systems, is part of planned series of boards, to provide full MSX2+ compliance.

It provides interface to keyboard and memory slot management to the ROM/RAM board.

Key Functions:

* Designed around a Renesas 82C55 chip, a modern CMOS version of the original Intel 8255 Programmable Peripheral Interface chip.
* IDC interface for full size MSX compatible keyboard.
* MSX Slot selector lines for the ROM/RAM board.
* Optional MSX M1 Wait state generator.

This board also has outputs for Cassette Audio out and Cassette Motor control, but I have yet to test this myself.

## Bill of Materials

|Count   | Name                    |  Designator |
|:------:|-------------------------|-------------|
| 5      | 0.1uF                   | C1,C3,C4,C2,C5 |
| 1      | 1N4148                  | D1 |
| 1      | EXT-BUS                 | J1 |
| 1      | M1-WAIT                 | J2 |
| 1      | ACTIVE                  | LED1 |
| 1      | MSX-KYB                 | P1 |
| 1      | 330 Ω                   | R1 |
| 1      | 82C55                   | U1 |
| 1      | 74HC138                 | U2 |
| 1      | 74HC00                  | U3 |
| 1      | 74HC153                 | U4 |
| 1      | 74HC74                  | U5 |
| 1      | Right Angle 20x2 Header | B1 |
| 1      | Right Angle header 1x20 | B1 |
| 1      | 14                      | POS IC SOCKET |
| 2      | 16                      | POS IC SOCKET |
| 1      | 40                      | POS IC SOCKET |

## Sample Apps

The msxkeyb.com application can be used to test the keyboard matrix lines are working correctly.

## Port Mapping

Standard MSX port mapping.

| Port |  Description
|:------:|-------------|
| #A8    | PSL_STAT: slot status |
| #A9    | KBD_STAT: keyboard status |
| #AA    | GIO_REGS: General IO Register |
| #AB    | PPI_REGS: PPI register |

For more see [msx.org wiki page](https://www.msx.org/wiki/Programmable_Peripheral_Interface)

## Using keyboard on RomWBW

Although this board is designed to work under MSX's software, it can be used in a stock RC2014 build.

You can write your own software to interface to the keyboard (see the demo app msxkeyb), or you can activate the
custom driver written for RomWBW's HBIOS.  The drivers code is on my fork of [RomWBW](https://github.com/vipoo/RomWBW/tree/dean/dev-v9958).

(Planning to submit this driver to the main project)

## Resources

* Schematic: [schematic.pdf](./schematic.pdf "Schematic")
* [82C55 Datasheet](../datasheets/82c55.pdf)
