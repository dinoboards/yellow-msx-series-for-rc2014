
# Yellow MSX for RC2014 ROM Image Builder

### MSX BIOS AND MSX-DOS

This sections contains the software required for the Yellow MSX Series of boards under the MSX BIOS/MSX-DOS systems

## Cloning this repo

This repo contains submodules.  So to clone the repo's submodules, you may need to do the following:

`git clone --recursive git@github.com:vipoo/yellow-msx-series-for-rc2014.git`

If you had already cloned it, without the `--recursive` switch:

`git submodule update --init`

> If using Windows Subsystem for Linux, make sure you are using the linux file system (not the mnt/c/... ntfs images)

## Installing Prerequisites

Before attempting to build any of the artifacts, you need to ensure you have the correct depdendencies ready:

* Linux (tested on ubuntu under WSL)
* (For the JED files, you will need a windows environment)
* [z88dk](https://github.com/z88dk/z88dk/wiki/installation)
* Bash 4.4
* make 4.1
* gcc
* Nextor support tools, see below
* [pasmo z80 assembler](https://pasmo.speccy.org/)

### Installing Nextor Support Tools

`make install-prereq`

## Building

Once all the prerequisites are done, you can build the entire system with just:

`make`

This should procduce the following artifacts in the `./msx/bin` directory

|          File               | Description                                                  |
|-----------------------------|--------------------------------------------------------------|
| cbios_main_rc2014_pal.rom*  | Main 32K CBIOS ROM                                           |
| cbios_logo_rc2014.rom*      | CBIOS' 16K Logo ROM                                          |
| cbios_sub.rom*              | Sub CBIOS ROM                                                |
| yellow-msx-pal-rc2104.rom   | CBIOS and Nextor 512K ROM for flashing onto Memory Board     |
| main.rom                    | Main 32 MSXSRCSYS ROM                                        |
| optrom.rom                  | MSXSRCSYS OPT or Logo ROM                                    |
| subrom.rom                  | MSXSRCSYS SUB ROM                                            |
| msxsyssrc-rc2014.rom        | MSXSRCSYS and Nextor 512K ROM for flashing onto Memory Board |
| nextor-2.1.1.rc2014.rom     | Nextor ROM Image                                             |
| extbio-rc2014.rom           | Custom Extended BIOS implementation for RC2014 platform      |
| chkspd. com                 | Testing tool                                                 |
| dots. com                   | Testing tool                                                 |
| extbio. com                 | Testing tool                                                 |
| fdisk. com                  | Testing tool                                                 |
| lines. com                  | Testing tool                                                 |
| mbrot. com                  | Testing tool                                                 |
| xrecv. com                  | Xmodem receive tools for the RC2014 SIO/2 Module             |


## Building the JED files:

The PLD logic required for the [ATF22V10](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0735.pdf) chip.

For building the `memory.jed` files, you need to have [Wincupl](https://www.microchip.com/en-us/products/fpgas-and-plds/spld-cplds/pld-design-resources) to compile the PLD logic (Windows or wine).

You will also need a programmer to code the chip -- I have used the [TL866II Plus from XGecu](http://www.xgecu.com/en/)

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

