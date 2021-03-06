# Yellow MSX for RC2014

This repo contains software support required for the Yellow MSX Series of boards

## Cloning this repo

This repo contains submodules.  So to clone the repo's submodules, you may need to do the following:

`git clone --recursive git@github.com:vipoo/msxrc2014.git`

If you had already cloned it, without the `--recursive` switch:

`git submodule update --init`


## memory.pld

The PLD logic required for the [ATF22V10](https://ww1.microchip.com/downloads/en/DeviceDoc/doc0735.pdf) chip.

For building the `memory.jed` files, you need to have [Wincupl](https://www.microchip.com/en-us/products/fpgas-and-plds/spld-cplds/pld-design-resources) to compile the PLD logic (Windows or wine).

You will also need a programmer to code the chip -- I have used the [TL866II Plus from XGecu](http://www.xgecu.com/en/)

## Sub-Modules

### CBIOS

For more details, see its README file:

[cbios/README.md](cbios/README.md)

### NEXTOR

For more details, see its README file:

the original [nextor/README.md](nextor/README.md)

and for the linux build process:

[nextor/linuxtools/README.md](nextor/linuxtools/README.md)

### Building

#### memory.jed

To assembly jed file, on a windows system:

`build-jed.bat`

Assumes wincupl has been installed at `c:\Wincupl`. You can set the environment variable to specify an alternative path:

```
set CUPLPATH=c:\mycuplpath\
build-jed.bat
```

If you are using Windows Subsystem for linux, you can invoke this script thru the make task:

`make memory.jed`

#### cbios

`make cbios`

#### nextor

Must install the required prerequisites with `make install-prereq`

there after:

`make nextor`



### Special Magic numbers

Makefile: dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom  of=bin/ymsx-pal.rom bs=16k count=**27** seek=4

Makefile-main.mk:	dd if=/dev/zero of=rc2014-driver-with-sectors.bin bs=16k count=**19** seek=0

Makefile-main.mk:	for i in {1..**18**}

Makefile-main.mk:	dd if=/dev/zero of=fdd.dsk bs=$$(($$DATSIZ* **18**)) count=1

embinc.mac: SECTOT	EQU	**19** *SECCNT


