# Yellow MSX Series for RC2014

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

The repo describes a series of boards designed for RC2014, to give MSX+ compatiblity.

At this stage, details of 2 boards are contained within.  More boards will be added as finialised.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

At the moment only the Video and Sound/Game boards are detailed - so the ability to run MSX software is currently limited.  In the meantime the boards can be used on a stock RC2014 with RomWBW/cpm OS.

The [apps-rc2014](./apps-rc2014) directory contains source code to test and explorer the functionality of these boards.

* dots.com - test the V9958 video board by drawing a series random dots
* lines.com - test the V9958 video board by drawing a series random lines
* mbrot.com - test the V9958 video board by slowly drawing a mandelbrot set
* control.com - test the GAME board by writing to serial out, as buttons are pressed on any attached joystick/game pads
* tune.com - copied from [RomWBW project](https://github.com/wwarthen/RomWBW). Play chiptune files on the GAME board - see the tunes directory for samples

The story of the prototyping and development these boards can be found at the Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014) project page.

## Acknowledgements

The design of the boards has borrowed a lot from pre-existing solutions out there, including:

* [S100 Computers VDP Project](http://www.s100computers.com/My%20System%20Pages/VDP%20Video%20Board/VDP%20Board.htm)
* [The MTXPlus+ Project](http://primrosebank.net/computers/mtx/projects/mtxplus/video/mtxplus_vdp_design.htm)
* [V9958 Video Board for rosco_m68k](https://github.com/rosco-m68k/hardware-projects/tree/master/video9958)
* [Steckschwein](https://steckschwein.de/hardware/v9958-video-board/)
* [The Artemisa Project](https://github.com/apoloval/artemisa)
* [The Omega MSX Kit](https://github.com/skiselev/omega)

*Standing on the sholders of giants.  Many thanks to all who share their knowledge and passions*

## Cloning this repo

This repo contains submodules.  So to clone the repo's submodules, you may need to do the following:

`git clone --recursive git@github.com:vipoo/yellow-msx-series-for-rc2014.git`

If you had already cloned it, without the `--recursive` switch:

`git submodule update --init`

> If using Windows Subsystem for Linux, make sure you are using the linux file system (not the mnt/c/... ntfs images)

**Please note: github submodule configuration assumes you are using ssh to access the repos**

If you get a permission denied error when attempting to clone the submodules, it may be due to your github access method.  The submodules are referenced using git ssh paths (eg:  git@github.com:vipoo/Nextor.git), as such if you attempt to clone these submodules using only https access, you may get a permission denied error.

To setup github access using ssl - follow the instruction on github at: https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh)

## Building

to build the RomWBW test applications:

```
cd apps-rc2014
make
```

### Building the MSX ROMS and MSX applications:

For more details see the [MSX README](./msx/README.md)


## V9958 Video Board Designed for RC2014

See the [README file here](./video/README.md)

Kit available at: https://www.tindie.com/products/dinotron/v9958-msx-video-board-for-rc2014/

## YM2149 Game Board Designed for RC2014

See the [README file here](./game/README.md)

Kit available at: https://www.tindie.com/products/dinotron/ym2149-msx-game-board-for-rc2014/

## Real-Time-Clock Board Designed for RC2014

See the [README file here](./rtc/README.md)

Comming Soon.

## PPI Board Designed for RC2014 & Keyboard

See the [PPI README file here](./ppi/README.md)

Kit available at: https://www.tindie.com/products/dinotron/msx-keyboard-designed-for-rc2014/

See the [Keyboard README file here](./keyboard/README.md)

Switches & Caps: at https://www.tindie.com/products/dinotron/switches-keycaps-for-msx-rc2014-keyboard/

## MSX Memory Board Designed for RC2014

See the [README file here](./memory/README.md)

Kit available at: https://www.tindie.com/products/dinotron/msx-memory-board-for-rc2014/

### MSX BIOS/MSX-DOS

The directory ./msx contains source and build scripts for building the ROM images.

See its [./msx/README.md](./msx/README.md) for more information.


## Other Board ideas

* Incorporate existing RC2014 boards, such as Floppy, IDE, Flash storage, Serial etc into a MSX system.
* Cassette Interface
* Cartridge slots
* Others....
## License
Copyright 2021 Dean Netherton

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

