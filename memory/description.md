

## Title

MSX Memory Board for RC2014

## Brief Description

Give your RC2014 the power of MSX BIOS and MSX-DOS/NEXTOR, with 512K ROM and upto 1024K RAM onboard RAM.


### What is it?

This is a kit, that once built and installed on your RC2014 platform along side the other MSX modules, will allow you to boot up MSX software on your RC2014.

### Key features

The kit is supplied with a flashed ROM containing:

* CBIOS, an open source version of MSX BIOS.

* NEXTOR, an officially sanctioned open source version of MSX-DOS (based on the original code).

* MSX-DOS driver for the RC2014 Compact Flash module.

* MSX-DOS driver for an embedded ROM DISK image, enabling disk-less bootup.

Also included on the PCB are optional pinouts for potential future RAM upgrade to full 4MB.

> For licensing reasons I am not able to distribute an image contain MSX BASIC.  The onboard ROM can be reflashed with your own custom build of MSX BIOS, which can include MSX-BASIC. Pre-build binaries and instructions are available on the [github repo's page](http://.....)

### What is the 'Yellow MSX Series of Boards'?

These are a series of boards developed to achieve MSX compatibility for RC2014 systems.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX games!

This Memory board in conjunction with Video, Sound, and PPI/Keyboard enable MSX bootup.

More details can be found on my [hackaday project](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

### What's included in this kit

The full kits includes everything you need (PCB, capacitors, IC sockets, and the ICs)

The SST39SF040 512K Flash ROM is supplied flashed with a build of CBIOS and MSX-DOS/Nextor - with your choice of PAL or NTSC versions.

Two new ATF22V10C programmed and ready for operation.

### What about MSX-BASIC?

Due to licencing and copyright restrictions, the ROM image supplied, contains the Open Source MSX compatible bios (CBIOS) and a custom build of NEXTOR/MSX-DOS.

This configuration does not include the MSX-BASIC language or runtime.

Despite that, CBIOS/NEXTOR will boot up to a MSX-DOS prompt.  This environment will allow for the running of many MSX-DOS/CPM applications, including SOFAROM to load and run game ROM images.  Although, naturally, anything that requires MSX-BASIC will not work.

But if you want to get the full MSX experience, then see the github project on instructions on creating a ROM image containing full MSX BASIC.

### What else do I need to make this work?

The Memory Board can not function without the PPI board.  And the MSX software will not work, unless there is a Video, Sound and Keyboard present.

* You need a RC2014 (Z80 CPU, clock, backplane pro).

You will also need to have a full complement of Yellow MSX Board:

* The V9958 MSX Video board for RC2014.

* YM2149 MSX Sound/Controller board for RC2014.

* PPI 82C55 MSX Interface board for RC2014, with Keyboard attached.


Recommended Modules:

* The RC2014 Compact Flash.

* The Dual clock.

* SIO/2 Serial.

### Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with.  I will do my best to answer any support questions you may have.

This board is not compatible with RomWBW and other memory modules.
