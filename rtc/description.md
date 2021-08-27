Title: RP5C01 RTC and MSX F4 board for RC2014

Descr: Give your RC2014 a MSX compatible Real time clock and MSX F4 Boot register

### What is it?

This kit, designed around the RP5C01 RTC chip, is part of the Yellow MSX series of modules to give your RC2014 MSX compatibility.

It gives your RC2014 the ability to remember the date and time when power is off.

The RPC501 chip maintains a time/date counter and when the machine is switched off, the chip is powered by a coin CR2032 battery.

In addition to keeping time, it also has a small amount of onboard RAM, that is also powered by the battery when main power is off.  This RAM can be used to store machine specific settings, such as boot screen color and mode.

### Key Features

* MSX2 compatible Real Time Clock
* Optional Reset on Power signal
* MSX2+ compatible F4 cold/boot register
* Compatible with RomWBW*

\* see RomWBW's dev branch

### Operation

Detail of building, configuring, calibrating the board can be found on the [github project page](https://github.com/vipoo/yellow-msx-series-for-rc2014/blob/main/rtc/README.md)

#### Reset on power

To enable this capability, you need to bridge jumper J2 (RESET).  Please see the notes on the github project regarding the RC2014 Dual Clock module's Reset on power function.

#### Calibration

The trimmer capacitor can be use to adjust the onboard clock for calibrating timing accuracy.  The MSX-DOS application `RTCCALB.COM` (downloadable from the github project page) can be used to show reference of the RTC to your CPU's clock

#### MSX F4 boot register

This is a single bit register at IO address $F4, that MSX software can use to identified if your machine was cold booted (powered on), or warm booted (reset).  MSX Bios software will skip boot screen and other startup code on a warm boot.

### What is the 'Yellow MSX Series of Boards'?

These are a series of boards that I am developing to achieve MSX compatibility for RC2014 systems.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

The first board in the series was the [V9958 Video board](https://www.tindie.com/products/dinotron/v9958-msx-video-board-for-rc2014/)

More details are available within the [hackaday project](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)




### What's included in this kit

The full kits includes (PCB, passive components, IC and their sockets, including the RP5C01 chip)

A battery is **not** included.  Requires a standard coin CR2032 battery.

### What else do I need to make this work?

* You need a working RC2014 system (CPU, Clock, etc).
* Enhanced Bus backplane required.

> For MSX operation, you need the other Yellow MSX series for RC2014 modules (PPI/Keyboard, ROM/RAM Memory, V9958 Video)

### Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with.  I will do my best to answer any support questions you may have.

The clock's timing should generally be adequate, but please don't expect super accurate time keeping.
