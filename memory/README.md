# MSX Memory Board for RC2014

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

## Description

This is a kit, that once built and installed on your RC2014 platform along side the other MSX modules, will allow you to boot up MSX software on your RC2014.

The onboard 512K ROM is mapped according to the MSX Architecture, so that the RC2014 can be booted with an MSX BIOS and MSX-DOS ROM image.

The ROM is paged into the Z80's address space, allowing the banking of MSX BIOS, MSX BIOS SUB-ROM, MSX BIOS LOGO BOOT along with a banked MSX-DOS/NEXTOR kernel and disk drivers for RC2014 operation.

The ROM is devided into 16K banks, mapped at the following addresses and MSX slots:

|  SLOT  |  CPU ADDRESS      |  DESC                    |  ROM CHIP'S ADDR    |
|--------|-------------------|--------------------------|---------------------|
|    0   | 0x0000 to 0x7FFF  | MAIN ROM (48K)           | 0x00000 to 0x07FFF  |
|    0   | 0x8000 to 0xBFFF  | LOGO ROM (16K)           | 0x08000 to 0x0BFFF  |
|  3-0   | 0x0000 to 0x3FFF  | SUB ROM (16K)            | 0x0C000 to 0x0FFFF  |
|  3-3   | 0x4000 to 0x7FFF  | MSX-DOS/NEXTOR/ROM DISK  | 0x10000 to 0x7FFFF  |

The first 64K of the onboard ROM is coded to be addressed from slot 0 and 3-0.
The remaining 28 16K banks mapped to slot 3-3, using the [ASCII16](https://www.msx.org/wiki/MegaROM_Mappers#ASC16_.28ASCII.29) banking system.

The banking configuration for the ROM is controlled thru a PLD (ATF22V10C), and can be reprogrammed to reconfigured the banking addressing, if so desired.

The source code and build scripts for the rom images can be found in this repo, under the [msx](./msx) directory.

### Key features

If you have purchased the kit from Tindie, then the ROM supplied will contain the following:
  * CBIOS, an open source version of MSX BIOS
  * NEXTOR, an officially sanctioned open source version of MSX-DOS (based on the original code)
  * MSX-DOS driver for the RC2014 Compact Flash module
  * MSX-DOS driver for an embedded ROM DISK image, enabling disk-less bootup

### Dependencies

This module must have the following MSX boards, (in addition to the standard RC2014 CPU, Clock and backplane pro modules)

* V9958 Video Board Designed for RC2014
* YM2149 GAME Board Designed for RC2014
* PPI Board Designed for RC2014
* MSX Keyboard for RC2014

The slot selection signals from the PPI board must be wired thru to the Memory board - see wiring notes below.

### Building your own custom bios image.

For configuring and building instructions for a custom rom, have a look at the instruction in the [msx/README.md](../msx/README.md) file

## Bill of Materials
|Count   | Name                      |  Designator |
|:------:|---------------------------|-------------|
| 2	     | 0.1uF                     | C1-C13      |
| 1      | BUS-MAP HEADER 2x7	       | J2          |
| 1      | ACTIVE                    | LED1        |
| 1      | 330 Ω                     | R1          |
| 2      | AS6C4008                  | U1, U13     |
| 1      | 74HCT153                  | U2          |
| 1      | 74HCT273                  | U3          |
| 1      | 74HC540	                 | U4          |
| 1      | SST39SF040                | U5          |
| 2      | 74HCT30                   | U6, U7      |
| 1      | 74HC138                   | U8          |
| 1      | ATF22V10C (RAM SELECTOR)  | U9          |
| 1      | ATF22V10C (ROM-MAPPER)    | U14         |
| 2      | 74HCT670                  | U10, U12    |
| 1      | Right Angle 20x2 Header   | B1          |
| 1      | Right Angle header 1x20   | B1          |
| 1	     | PCB                       |             |

## Operating Instruction

Once you have been able to boot up your RC2014 under MSX-DOS, you can try a few commands from the diskless embedded floppy image:

If you have used MS-DOS or CP/M you may find that MSX-DOS feel quite familiar. MSX-DOS has a degree of compatibility with standard CP/M.  Many CP/M programs will work just fine.

But MSX-DOS extends CP/M to provide support for subdirectories, and other features found in early MS-DOS.

Some things to try out and get a feel for your MSX on RC2014 system.

| Command               | Description |
|-----------------------|-------------|
| `DIR`                   | Shows list of directores/files in current drive/directory |
| `MEMTEST`               | will conduct a test of the RAM installed         |
| `TYPE AUTOEXEC.BAT`   | Show the contents of the AUTOEXEC.BAT file                |
| `CD SYSTEM`             | Change into a directory called system, here you will see lots of utilities you can run |
| `HELP`                  | Show the embedded commands available in at the MSX-DOS prompt |
| `BASIC`                   | If your ROM contains basic, jumps to the BASIC interpreter |

But for real fun, try this:

```
CD ROMS
SROM PACMAN.ROM
```

### Partitioning the Compact Flash Module

TBD - use the fdisk.com tool

### Xmodem for SIO/2

TBD - use the xrecv.com tool

### Images

![Assembled](./PXL_20210410_071727624.jpg)
![PCB](./PXL_20210416_125150502.jpg)

## Wiring Requirements

The memory board needs to have the slot selection signals mapped from the MSX PPI board.  Although some of the extended bus lines are used, typically the backplane pro may not map all signals.

You need to ensure the bus map signals for SLT_LOW and SLT_HIGH are connected between the two boards.  Use a standard jumper wire if you backplane does not map these lines.

![Image showing external wiring mapping the 2 boards](./PXL_20210410_072253452.jpg)

As the slot selection signals are produced by the PPI interface board,
The Memory board requires a wiring to the PPI Interface board.
## Resources

* Schematic: [schematic.pdf](./schematic.pdf "Schematic")

