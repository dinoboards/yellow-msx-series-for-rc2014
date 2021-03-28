

## Title

MSX MEMORY Module for RC2014

## Brief Description

Give your RC2014 the power of MSX BIOS and MSX-DOS/NEXTOR, with 512K ROM and upto 1024K RAM onboard RAM.

### What is it?

This is a kit, that once built and installed on your RC2014 platform along side the other MSX modules, will allow you to boot up MSX software on your RC2014.

The onboard ROM has been divided and paged so that it can map the MSX BIOS, MSX SUB-ROM, MSX LOGO bootup screen, and the banked MSX-DOS/Nextor kernel with disk drivers for RC2014 operation.

The onboard ROM is devided into 16K banks, mapped at the following addresses and slots:

|  SLOT  |  CPU ADDRESS      |  DESC                    |  ROM CHIP'S ADDR    |
|--------|-------------------|--------------------------|---------------------|
|    0   | 0x0000 to 0x7FFF  | MAIN ROM (48K)           | 0x00000 to 0x07FFF  |
|    0   | 0x8000 to 0xBFFF  | LOGO ROM (16K)           | 0x08000 to 0x0BFFF  |
|  3-0   | 0x0000 to 0x3FFF  | SUB ROM (16K)            | 0x0C000 to 0x0FFFF  |
|  3-3   | 0x4000 to 0x7FFF  | MSX-DOS/NEXTOR/ROM DISK  | 0x10000 to 0x7FFFF  |

The first 64K of the onboard ROM is coded to be addressed from slot 0 and 3-0.
The remaining 28 16K banks mapped to slot 3-3, using the [ASCII16](https://www.msx.org/wiki/MegaROM_Mappers#ASC16_.28ASCII.29) banking system.

The banking configuration for the ROM is controlled thru a PLD (ATF22V10C), and could be reprogrammed to reconfigured the banking addressing, if so desired.

### Key features

The kit is supplied with a flashed ROM containing:
  * CBIOS, an open source version of MSX BIOS
  * NEXTOR, an officially sanctioned open source version of MSX-DOS (based on the original code)
  * MSX-DOS driver for the RC2014 Compact Flash module
  * MSX-DOS driver for an embedded ROM DISK image, enabling disk-less bootup

Also included on the PCB are optional pinouts for potential future RAM upgrade to full 4MB.

### What is the 'Yellow MSX Series of Boards'?

These are a series of boards developed to achieve MSX compatibility for RC2014 systems.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX games!

This Memory board in conjuction with Video, Sound, and PPI/Keyboard enable MSX bootup.

More details can be found on my [hackaday project](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

### What's included in this kit

The full kits includes everything you need (PCB, capacitors, IC sockets, and the ICs)

The SST39SF040 512K Flash ROM is supplied flashed with a build of CBIOS and MSX-DOS/Nextor - with your choice of PAL or NTSC versions.

Two new ATF22V10C programmed and ready for operation.

### What about MSX-BASIC?

Due to licencing and copyright restrictions, the ROM image supplied, contains the Open Source MSX compatible bios (CBIOS) and a custom build of NEXTOR/MSX-DOS.

This configuration does not include the MSX-BASIC language or runtime.

Despite that, CBIOS/NEXTOR will boot up to a MSX-DOS prompt.  This enviornment will allow for the running of many MSX-DOS/CPM applications, including SOFAROM to load and run game ROM images.  Although, naturally, anything that requires MSX-BASIC will not work.

But if you want to get the full MSX experience, then see the github project on instructions on creating a ROM image containing full MSX BASIC.

### What else do I need to make this work?

The Memory Board can not function without the PPI board.  And the MSX software will not work, unless there is a Video, Sound and Keyboard present.

* You need a RC2014 (Z80 CPU, clock, backplane pro).

You will also need to have a full complement of Yellow MSX Board:
* The V9958 MSX Video board for RC2014
* YM2149 MSX Sound/Controller board for RC2014
* PPI 82C55 MSX Interface board for RC2014, with Keyboard attached

Recommended Modules:
* The RC2014 Compact Flash
* The Dual clock
* SIO/2 Serial

### Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with.  I will do my best to answer any support questions you may have.

This board is not compatible with RomWBW and other memory modules.
