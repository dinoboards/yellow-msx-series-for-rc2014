# Yellow MSX Series for RC2014

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

The repo describes a series of boards designed for RC2014, to give MSX+ compatiblity.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

The story of the prototyping and development these boards can be found at the Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014) project page.

## Configuration and Software Notes

Click thru for more information on:

**[RomWBW Configuration](./readme-romwbw.md) specific instructions**

**[Recommended RC2014 Configuration for MSX modules](./readme-msx.md)**

**[General MSX documents](./msx/docs/index.md)**

**[Details of compiling and assembling](./msx/README.md) MSX applications and bios images**


## Hardware, construction and module specific Instructions

#### V9958 MSX Video Modules for RC2014 (Advanced and RGB versions)

See the [README file here](./video/README.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/v9958-msx-video-advanced-module-for-rc2014) store

#### YM2149 Game Module Designed for RC2014

See the [README file here](./game/README.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/ym2149-msx-game-board-for-rc2014) store

#### Real-Time-Clock Module Designed for RC2014

See the [README file here](./rtc/README.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/msx-rp5c01-rtc-and-msx-f4-board-for-rc2014) store

#### PPI Module Designed for RC2014 & Keyboard

See the [PPI README file here](./ppi/README.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/msx-keyboard-designed-for-rc2014) store

See the [Keyboard README file here](./keyboard/README.md)

Switches & Caps available at the [Tindie](https://www.tindie.com/products/dinotron/switches-keycaps-for-msx-rc2014-keyboard) store

#### MSX Memory Module Designed for RC2014

See the [README file here](./memory/README.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/msx-memory-board-for-rc2014) store

#### YM2413 MSX-Music Module Designed for RC2014

See the [README file here](./music/README.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/ym2413-msx-music-module-designed-for-rc2014) store

#### MSX Cartridge Slot Extension for RC2014 Bus

See the [README file here](./slot-extension/readme.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/msx-cartridge-slot-extension-for-rc2014-bus) store

#### 12+1 Backplane designed for RC2014 systems

See the [README file here](./backplane/readme.md)

Kit available at [Tindie](https://www.tindie.com/products/dinotron/121-backplane-designed-for-rc2014-systems) store

#### Integrating the ESP8266 Wifi Module

Click [here](./wifi/README.md) for instructions on integrating the stock RC2014 ESP8266 wifi module into your MSX build.

Kits are available from the official RC2014 stores at [z80Kits](https://z80kits.com/shop/esp8266-wifi-module) and [Tindie](https://www.tindie.com/products/semachthemonkey/esp8266-wifi-module-for-rc2014/)


## Other Board ideas

* Incorporate existing RC2014 boards, such as Floppy, IDE, Flash storage, Serial etc into a MSX system.
* Cassette Interface
* Others....

## Cloning this repo

This repo contains submodules.  So to clone the repo's submodules, you may need to do the following:

`git clone --recursive git@github.com:vipoo/yellow-msx-series-for-rc2014.git`

If you had already cloned it, without the `--recursive` switch:

`git submodule update --init`

> If using Windows Subsystem for Linux, make sure you are using the linux file system (not the mnt/c/... ntfs images)

**Please note: github submodule configuration assumes you are using ssh to access the repos**

If you get a permission denied error when attempting to clone the submodules, it may be due to your github access method.  The submodules are referenced using git ssh paths (eg:  git@github.com:vipoo/Nextor.git), as such if you attempt to clone these submodules using only https access, you may get a permission denied error.

To setup github access using ssl - follow the instruction on github at: https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh)

## Acknowledgements

The design of the boards has borrowed a lot from pre-existing solutions out there, including:

* [S100 Computers VDP Project](http://www.s100computers.com/My%20System%20Pages/VDP%20Video%20Board/VDP%20Board.htm)
* [The MTXPlus+ Project](http://primrosebank.net/computers/mtx/projects/mtxplus/video/mtxplus_vdp_design.htm)
* [V9958 Video Board for rosco_m68k](https://github.com/rosco-m68k/hardware-projects/tree/master/video9958)
* [Steckschwein](https://steckschwein.de/hardware/v9958-video-board/)
* [The Artemisa Project](https://github.com/apoloval/artemisa)
* [The Omega MSX Kit](https://github.com/skiselev/omega)
* [MSX ORG Forums](https://www.msx.org/)

*Standing on the shoulders of giants.  Many thanks to all who share their knowledge and passions*

## License
Copyright 2021 Dean Netherton

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

