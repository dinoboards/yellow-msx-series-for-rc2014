
# Yellow MSX for RC2014 ROM Image Builder

[![CircleCI](https://circleci.com/gh/vipoo/yellow-msx-series-for-rc2014/tree/main.svg?style=svg)](https://circleci.com/gh/vipoo/yellow-msx-series-for-rc2014/tree/main)

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
* A driver for the RC2014 Compact Flash Module - Spencer's Kits at [Z80Kits](https://z80kits.com/shop/compact-flash-module) and [Tindie](https://www.tindie.com/products/semachthemonkey/compact-flash-module-for-cpm-rc2014-computer)

There are plans to add additional RC2014 hardware support, including Floppy drives, Serial and others.

[README](https://github.com/vipoo/Nextor/blob/dean/1/v2.1/README.md)

### MSXSYSSRC

Although MSX-DOS has effectively been opened source thru the NEXTOR system, and Microsoft open sourcing GW-BASIC and early versions of MS-DOS - its a disappointment to many that the MSX-BIOS/BASIC has not also been opened source.

Given this environment, the MSXSYSSRC project is an attempt to reverse engineer the various MSX official ROMS, so that the retro community can understand and educate themselves as to how it was all done back then.

https://sourceforge.net/projects/msxsyssrc/

## Updating your ROM

You can assembly a new ROM image by following the instructions below, or download one of the pre-assembled [released packages](https://github.com/vipoo/yellow-msx-series-for-rc2014/releases)

There are 2 different ways to compile and assemble the rom.  Docker or host.  With Docker, you only need to have docker installed.  Alternatively, you can install the full tool chain (z88dk, pasmo, linux packages etc)
## Building the binaries using the prebuilt docker image

After you have installed docker on your system, you can compile the ROM image using the following command from within the `msx` directory:

```
docker run -v ${PWD}:/src/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it vipoo/yellow-msx-rc2014-tool-chain:latest make
```

The docker run command is complex, so to make it easier, make an alias entry (typically in something like `~/.bash_aliases`)

```
alias msxmake="docker run -v \${PWD}:/src/ --privileged=true -u \$(id -u \${USER}):\$(id -g \${USER}) -it vipoo/yellow-msx-rc2014-tool-chain:latest make"
```

There after, you can treat the alias as an alternative make command eg:

```
msxmake clean
msxmake apps
````

If you prefer to avoid using Docker, the following sections describe a more native build process.

## Building the binaries manually

The Makefile in this directory can orchestrate the building of all binary units required for the RC2014 MSX system.

After installing and configuring the prerequisites, simply run `make` in the msx directory.

## Installing Prerequisites

Before attempting to build any of the artifacts, you need to ensure you have the correct dependencies ready:

* Linux (tested on ubuntu under WSL)
* (For the JED files, you will need a windows environment)
* [z88dk](https://github.com/z88dk/z88dk/wiki/installation)
* Bash 4.4 or greater
* make 4.1 or greater
* gcc (build-essentials)
* mtools
* Nextor support tools, see below
* [pasmo z80 assembler](https://pasmo.speccy.org/)

> PASMO: For debian based linux, just run `sudo apt install pasmo`

### Installing Nextor Support Tools

`make install-prereq`

## Compiling/Assembling

Once all the prerequisites are done, you can build the entire system with just:

`make`

This should produce the following artifacts in the `./msx/bin` directory

#### ROM Flashing units

These binaries are for flashing onto the SST39SF040 ROM for the MSX Memory Module:

|          File                              | Description                                                       |
|--------------------------------------------|-------------------------------------------------------------------|
| yellow-msx-pal-rc2104.rom                  | The CBIOS based build configured for 50Hz                         |
| yellow-msx-ntsc-rc2104.rom                 | The CBIOS based build configured for 60Hz                         |
| msxsyssrc-rc2014-<country>-with-rtc.rom    | The MSXSYS based build per country settings, with RTC enabled     |
| msxsyssrc-rc2014-<country>-without-rtc.rom | The MSXSYS based build per country settings, with RTC disabled    |

> The *without-rtc* versions are required for configuration without the RTC module - otherwise system will not start

> The various country settings can be found in [./msxsys-build/base300.inc](./msxsys-build/base300.inc) - and are not fully tested.


#### Other files produced are:

|          File               | Description                                                  | Address   | Emulator Slot | RC2014 Slot |
|-----------------------------|--------------------------------------------------------------|-----------|---------------|-------------|
| cbios_main_rc2014_pal.rom   | Main 32K CBIOS ROM                                           | 0000-7FFF |       0       |      0      |
| cbios_main_rc2014_ntsc.rom  | Main 32K CBIOS ROM                                           | 0000-7FFF |       0       |      0      |
| cbios_logo_rc2014.rom       | RC2014 Custom' 16K Logo ROM                                  | 8000-9FFF |       0       |      0      |
| cbios_sub.rom               | Sub CBIOS ROM                                                | 0000-3FFF |      3-0      |     3-0     |
| main.rom                    | Main 32K MSXSRCSYS ROM                                       | 0000-7FFF |       0       |      0      |
| optrom.rom                  | MSXSRCSYS OPT or Logo ROM                                    | 4000-7FFF |               |             |
| subrom.rom                  | MSXSRCSYS SUB ROM                                            | 0000-3FFF |      3-0      |     3-0     |
| rc2014.nextor-2.1.1.rom     | Nextor ROM Image                                             | 4000-7FFF |       2       |     3-3     |
| rc2014-extended.rom         | Custom Extended BIOS implementation for RC2014 platform      | 8000-CFFF |      3-3      |     3-3     |
| dots.com                    | Testing tool                                                 |           |               |             |
| extbio.com                  | Testing tool                                                 |           |               |             |
| fdisk.com                   | Testing tool                                                 |           |               |             |
| lines.com                   | Testing tool                                                 |           |               |             |
| mbrot.com                   | Testing tool                                                 |           |               |             |
| xrecv.com                   | Xmodem receive tools for the RC2014 SIO/2 Module             |           |               |             |
| rtccalb.com                 | Tool to calibrate RTC module against CPU Clock               |           |               |             |

See Also [Apps and Utils for MSX on RC2014](./docs/apps.md)

## Building your own custom ROM images

This section describes how you can create your own custom MSX Rom image that includes MSX BASIC.

After verifying you have been able to build as per the instruction above, you can follow these steps for your custom build:

1. If you would like to include any files in the embedded floppy image, simply copy them to `nextor/extras` directory*

2. run `make` (or the docker `msxmake` alias) and ensure no assembly errors are raised.

3. Flash the appropriate ROM image - *msxsyssrc-rc2014-\<country\>-without-rtc.rom* or *msxsyssrc-rc2014-\<country\>-with-rtc.rom*.

4. Boot up your new system.

\* Free space is limited on the embedded floppy image, you may need to update the Makefile to avoid copying additional files into the extras directory.

## Building the JED files:

Some modules, use Programmable Logic Device chips (PLD) - in particular there are 2 types used across various MSX modules.  The [ATF22V10](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0735.pdf) chip and variants of [ATF16V8x](https://ww1.microchip.com/downloads/en/devicedoc/atmel-0425-pld-atf16v8c-datasheet.pdf).

Microchip's [Wincupl](https://www.microchip.com/en-us/products/fpgas-and-plds/spld-cplds/pld-design-resources) is used to compile PLD to JED files.  The JED files can then be, using a programmer such as the [TL866II Plus from XGecu](http://www.xgecu.com/en/) flashed/programmed into the chips.

> For the ATF22V10C PLDS, make sure you select the IC type of `ATF22V10C(UES)` to ensure all fuses are applied.  For the ATF16V8x chips, make sure you choose the correct version (ATF16V8B or ATF16V8C).   I found out the hard way that other ATF22V10C types listed will for the most part work - but a few fuses will not be applied, and your CPLD will be slightly wrong

You can use the `build-jed.bat` to assist with compiling the PLD files.

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

There are also other pld files for other modules - including the V9958 Video Module and the cartridge slot extension.

### Special Magic numbers

```
Makefile: dd conv=notrunc status=none if=./nextor/bin/rc2014.nextor-2.1.1.rom  of=bin/ymsx-pal.rom bs=16k count=**27** seek=4

Makefile-main.mk:	dd if=/dev/zero of=rc2014-driver-with-sectors.bin bs=16k count=**19** seek=0

Makefile-main.mk:	for i in {1..**18**}

Makefile-main.mk:	dd if=/dev/zero of=fdd.dsk bs=$$(($$DATSIZ* **18**)) count=1

embinc.mac: SECTOT	EQU	**19** *SECCNT
```
