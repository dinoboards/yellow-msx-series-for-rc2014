
# Yellow MSX for RC2014 ROM Image Builder

### MSX BIOS AND MSX-DOS

This sections contains the software required for the Yellow MSX Series of boards allowing for MSX BIOS bootup.

## Cloning this repo

This repo contains submodules.  So to clone the repo's submodules, you may need to do the following:

`git clone --recursive git@github.com:vipoo/yellow-msx-series-for-rc2014.git`

If you had already cloned it, without the `--recursive` switch:

`git submodule update --init`

> If using Windows Subsystem for Linux, make sure you are using the linux file system (not the mnt/c/... ntfs images)

## MSX Information

See the [MSX doc index](./docs/index.md) for various documents pertaining to MSX

## Subsections of this repo

### C-BIOS

This submodule contains a fork of the C-BIOS open source alternative to MSX BIOS.  It does not contain MSX BASIC, but does provide enough capability to allow the RC2014 to boot into MSX-DOS.

But due to the lack of MSX-BASIC, some applications may not work.

[README](https://github.com/vipoo/cbios/tree/main)

http://cbios.sourceforge.net/

### NEXTOR

This submodule contains a fork of the NEXTOR disk operating system for MSX system.  NEXTOR can trace is source code history to the original and official MSX-DOS systems.

This local fork, also includes drivers specifically for RC2014, including:

* An embedded floppy boot image, allowing the RC2014 to be booted diskless, without the need for any external storage
* A driver for the [RC2014 Compact Flash module](https://www.tindie.com/products/semachthemonkey/compact-flash-module-for-cpm-rc2014-computer/)

There are plans to add additional RC2014 hardware support, including Floppy drives, Serial and others.

[README](https://github.com/vipoo/Nextor/blob/dean/1/v2.1/README.md)

### MSXSYSSRC

Although MSX-DOS has effectively been opened source thru the NEXTOR system, and Microsoft open sourcing GW-BASIC and early versions of MS-DOS - its a disappointment to many that the MSX-BIOS/BASIC has not also been opened source.

Given this environment, the MSXSYSSRC project is an attempt to reverse engineer the various MSX official ROMS, so that the retro community can understand and educate themselves as to how it was all done back then.

https://sourceforge.net/projects/msxsyssrc/

## Updating your ROM

You can assembly a new ROM image by following the instructions below, or download one of the pre-assembled [released packages](https://github.com/vipoo/yellow-msx-series-for-rc2014/releases)
## Building the binaries

The Makefile in this directory can orchestrate the building of all binary units required for the RC2014 MSX system.

After installing and configuring the prerequisites, simply run `make` in the msx directory.

## Installing Prerequisites

Before attempting to build any of the artifacts, you need to ensure you have the correct dependencies ready:

* Linux (tested on ubuntu under WSL)
* (For the JED files, you will need a windows environment)
* [z88dk](https://github.com/z88dk/z88dk/wiki/installation)
* Bash 4.4 or greater
* make 4.1 or greater
* gcc
* Nextor support tools, see below
* [pasmo z80 assembler](https://pasmo.speccy.org/)

> PASMO: For debian based linux, just run `sudo apt install pasmo`

### **Manual patch required to z88dk**

At this time, I have had to manually patch my local copy of z88dk to be able to compile MSX C applications.  I have submitted a patch to the z88dk team, but until thats accepted, you will need to manually tweak your local copy of the `msx.h` file.  See [https://github.com/z88dk/z88dk/pull/1787/files](https://github.com/z88dk/z88dk/pull/1787/files).  You dont need to compile z88dk - just change the header file on your local system.  If the patch or other fix has since been applied, this can be ignored.

### Installing Nextor Support Tools

`make install-prereq`

## Compiling/Assembling

Once all the prerequisites are done, you can build the entire system with just:

`make`

This should produce the following artifacts in the `./msx/bin` directory

|          File               | Description                                                  | Address   | Emulator Slot | RC2014 Slot |
|-----------------------------|--------------------------------------------------------------|-----------|---------------|-------------|
| cbios_main_rc2014_pal.rom*  | Main 32K CBIOS ROM                                           | 0000-7FFF |       0       |      0      |
| cbios_main_rc2014_ntsc.rom* | Main 32K CBIOS ROM                                           | 0000-7FFF |       0       |      0      |
| cbios_logo_rc2014.rom*      | RC2014 Custom' 16K Logo ROM                                  | 8000-9FFF |       0       |      0      |
| cbios_sub.rom*              | Sub CBIOS ROM                                                | 0000-3FFF |      3-0      |     3-0     |
| yellow-msx-pal-rc2104.rom** | CBIOS and Nextor 512K ROM for flashing onto Memory Board     |           |               |             |
| main.rom*                   | Main 32K MSXSRCSYS ROM                                       | 0000-7FFF |       0       |      0      |
| optrom.rom*                 | MSXSRCSYS OPT or Logo ROM                                    | 4000-7FFF |               |             |
| subrom.rom*                 | MSXSRCSYS SUB ROM                                            | 0000-3FFF |      3-0      |     3-0     |
| msxsyssrc-rc2014.rom**      | MSXSRCSYS and Nextor 512K ROM for flashing onto Memory Board |           |               |             |
| nextor-2.1.1.rc2014.rom*    | Nextor ROM Image                                             | 4000-7FFF |       2       |     3-3     |
| rc2014-extended.rom*        | Custom Extended BIOS implementation for RC2014 platform      | 8000-CFFF |      3-3      |     3-3     |
| chkspd. com                 | Testing tool                                                 |           |               |             |
| dots. com                   | Testing tool                                                 |           |               |             |
| extbio. com                 | Testing tool                                                 |           |               |             |
| fdisk. com                  | Testing tool                                                 |           |               |             |
| lines. com                  | Testing tool                                                 |           |               |             |
| mbrot. com                  | Testing tool                                                 |           |               |             |
| xrecv. com                  | Xmodem receive tools for the RC2014 SIO/2 Module             |           |               |             |

> \* used only by emulator

> \*\* For flashing on SST39SF040 ROM of the Memory board

## Building your own custom image

This section describes how you can create your own custom MSX Rom image that includes MSX BASIC.

After verifying you have been able to build as per the instruction above, you can follow these steps for your custom build:

1. Update the file [msx/msxsys-build/base300.inc](./msxsys-build/base300.inc) to enable or disable language, refresh rates, etc, for your specific requirements.

2. If you would like to include any files in the embedded floppy image, simply copy them to `nextor/extras` directory*

2. run `make` and ensure no assembly errors are raised.

3. Flash the msxsyssrc-rc2014.rom file onto your ROM.

4. Boot up your new system.

\* Free space is limited on the embedded floppy image, you may need to update the Makefile to avoid copying additional files into the extras directory.

## Building the JED files:

The PLD logic required for the [ATF22V10](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0735.pdf) chip.

For building the `memory.jed` files, you need to have [Wincupl](https://www.microchip.com/en-us/products/fpgas-and-plds/spld-cplds/pld-design-resources) to compile the PLD logic (Windows or wine).

You will also need a programmer to code the chip -- I have used the [TL866II Plus from XGecu](http://www.xgecu.com/en/).  Make sure you select the IC type of `ATF22V10C(UES)` to ensure all fuses are applied.

> I found out the hard way that other ATF22V10C types listed will for the most part work - but a few fuses will not be applied, and your CPLD will be slightly wrong

`build-jed.bat <filename without extension>`

Assumes wincupl has been installed at `c:\Wincupl`. You can set the environment variable to specify an alternative path:

```
set CUPLPATH=c:\mycuplpath\
build-jed.bat rom-mapper.pld
```

There are 2 PLD files required for the Memory board

### rom-mapper.pld

Provided rom address mapping.  See the rom-mapper.pld for details on the bank/address/slot mapping.

### mem-selector.pld

Provides for general control signals.



### Special Magic numbers

Makefile: dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom  of=bin/ymsx-pal.rom bs=16k count=**27** seek=4

Makefile-main.mk:	dd if=/dev/zero of=rc2014-driver-with-sectors.bin bs=16k count=**19** seek=0

Makefile-main.mk:	for i in {1..**18**}

Makefile-main.mk:	dd if=/dev/zero of=fdd.dsk bs=$$(($$DATSIZ* **18**)) count=1

embinc.mac: SECTOT	EQU	**19** *SECCNT

