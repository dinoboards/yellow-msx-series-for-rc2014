## References

http://fabf38.free.fr/diy/upgrmsx2/ct80msx2.png


## Title

V9958 MSX RGB Video Module for RC2014

## Brief Description

Give your RC2014 the advanced graphics of the late 80s.  Based around the V9958 chip, used in the MSX2+ line of computers.  Output through a VGA/DSUB connector.

## Full Description

### What is it?

A complete kit to give your RC2014 the advanced graphic capabilities of the mid to late 80s. It's based on the V9958 Video Display Processor (VDP) which powered
the MSX2+ line of 8 bit Z80 computers.

This V9958 chip, was produced by Yamaha in the late 80s and was an update to the their V9938 chip, which was itself, a successor to TI's TMS9918.  The V9938/V9958 addressed many of the shortcomings of the original TMS9918.

### Some of the V9958 key features include:

* Hardware acceleration (line and dot commands, horizontal scrolling registers, and others)
* Support for up to 19268 colours
* 13 Screen Modes
* Resolutions up to 512 x 212


### Key features of this board

* Compatible with RC2014 (enhanced bus required)
* 128K of RAM
* RGB 15Khz output via standard 15 pin DSUB/VGA connector for easy connectivity

### Alternative Video Modules

An alternative version of this kit, the [V9958 Advanced Module](https://www.tindie.com/products/dinotron/v9958-msx-video-advanced-module-for-rc2014), is available that in addition to the RGB output also produces composite and s-video output.

*Please note, that the RGB output of the Advance Module is via a custom mini-din pin connector, and would likely require the making of a custom cable for RGB operation.*

### Video conversion/display options

The 15 pin DSUB/VGA connector, makes for a simple and easy solution for any up-converters that accepts a DSUB/VGA input, such as the excellent and low cost arcade based upscalers available on ebay, amazon and other places.

(Some VGA monitors may be able to process the output - but most will not, as the output is a lower resolution 15kHz signal.)

The [RetroRGB upscalers article](https://www.retrorgb.com/upscalers.html) has a good description for some of the converter options.

Items such as retroTink and OSSC are very high quality converters - but also have a decent price tag.

I have found the low cost arcade converters such as the GBS-8200 and HD-VC9900 converters produce excellent results.  The upscalers can be acquired through the usual ebay, amazon and other sellers at very reasonable prices.

The GBS-8200 can also be easily modified to produce more customisable and higher quality output - again RetroRGB has it all explained at [GBS Control](https://www.retrorgb.com/gbs-control-installation-overview.html)

If you plan to use the GBS8200 or HD-VC9900 converters, then this kit is the one you need.


### What is the 'Yellow MSX Series of Boards'?

These are a series of boards that I am developing to achieve MSX compatibility for RC2014 systems.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

The V9958 board is the first in that series.

More details can be found on my [hackaday project](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

### What's included in this kit

The kit can be supplied with individually tested *V9958* and *4164C* chips.  Or you can choose to exclude these chips if you wish to source them yourself.

Everything else you need is included (PCB, passive components, crystals, sockets, connectors and other ICs)

The ATF16V8 is supplied new and programmed for immediate operation.

### What else do I need to make this work?

* You need a working RC2014 system (cpu, RomWBW, Clock, etc - enhanced bus required).
* A compatible monitor or upscaler converter

### Construction notes

Please note that due to the finer pitch pins of the V9958, the soldering of this board requires a little more finesse than the typical RC2014 through-hole based boards.  If you have built a few boards already, then you should be fine.  It's recommended that a finer pitch soldering tip be used as it can be easy to accidentally bridge some of the pins.

Extra care needed when inserting the V9958 into its socket - there are lots of pins, take your time to avoid bending pins.

For more notes and any revisions please have a look at the [github page](https://github.com/vipoo/yellow-msx-series-for-rc2014/blob/main/video/README.md)

### Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with.  I will do my best to answer any support questions you may have.

The V9958 and 41464C chips are obsolete and out of manufacture.  They may be old-new-stock or pre-used, and as such there is a risk they may not function correctly.  Therefore, I fully test each chip, in-circuit, prior to packaging, so you can have good confidence that the chips you receive will perform as required.


