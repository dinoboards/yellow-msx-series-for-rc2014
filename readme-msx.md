# RomWBW Configuration

## Recommended RC2014 Configuration

You should be able to install the V9958, Game and keyboard modules into any stock RC2014 builds.  RomWBW now has support for the keyboard and video modules.

If you wish to operate in MSX Compatibility module, in addition to the GAME, VIDEO, KEYBOARD, you will need the MEMORY module and the following stock RC2014 modules (configured as described):

1. **[Enhanced CPU](https://rc2014.co.uk/modules/cpu/z80-cpu-v2-1/)** - you need a CPU of course - running at 3.5Mhz.  The CBIOS image can operate at the 7Mhz rate, but many applications will not work at this frequency.  The MSXSYS based builds only work when the CPU is clocked at 3.5MHz

2. **[Dual Clock](https://rc2014.co.uk/modules/dual-clock-module/)**.  Dual clock is recommended so that the SIO/2 module's 2nd serial port can be clocked down.  The MSX bios will struggle to operate the SIO/2 chip if both CPU and SIO/2 clock lines are the same.  You will want to set the primary clock to 3.5Mhz and the 2nd clock to 0.3072Mhz. (See notes below for specific difference for bios msx versions*)

3. **[Dual Serial](https://rc2014.co.uk/modules/dual-serial-module-sio2/)**.  If you want to send files over a serial link you will need a serial module.  At this stage the software image only has a driver for the SIO/2 module. The Driver only uses the 2nd serial port - the first is unused.  Configuring the 2nd Clock at 0.3072Mhz will map to a max and default baud rate of 19200.  Please note that this mapping of clock frequency to baud rate is a little different than RomWBW configuration.  At a clock of 0.3072Mhz, the software can select a baud rate of 19200, 9600 or 4800.

4. **[Compact Flash](https://rc2014.co.uk/modules/compact-flash-module/)**:  The system will boot up without a CF module installed, and you can use the RAM disk (H:) to transfer files etc. But if you want to be able to save files, you will need the stock CF Module.

> \*SIO/2 Driver versioning.  Please note that for BIOS version 21-05-08, the dual serial clock rates are as per RomWBW - for 19200 Baud, select a clock of 1.2288Mhz.  For build 2021-07-24 and after, follow settings as described above (eg: 0.3072Mhz => 19200)


The official RC2014 kits can be purchased at Spencer Owen's official stores at [Tindie](https://www.tindie.com/stores/semachthemonkey/items/) or [Z80kits.com](https://z80kits.com)

## General MSX Information

See the [MSX doc index](./msx/docs/index.md) for various documents pertaining to MSX

## Building the MSX ROMS and MSX applications:

Details for compiling and assembling applications and bios images can be found [here](./msx/README.md)

[Back](./README.md)
