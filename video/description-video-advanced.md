

## Title

V9958 MSX Video Advanced Module for RC2014

## Brief Description

Give your RC2014 the advanced graphics of the late 80s.  Based around the V9958 chip, used in the MSX2+ line of computers.

## Full Description

### What is it?

A complete kit to give your RC2014 the advanced graphic capabilities of the mid to late 80s. It's based on the V9958 Video Display Processor (VDP) which powered
the MSX2+ line of 8 bit Z80 computers.

This V9958 chip, was produced by Yamaha in the late 80s and was an update to the their V9938 chip, which was itself, a successor to TI's TMS9918.  The V9938/V9958 addressed many of the shortcomings of the original TMS9918.

Please note, the original revision of 1.4 is no longer supplied, and the new and enhanced revision 1.9 is the current revision.

### Some of the V9958 key features include:

* Hardware acceleration (line and dot commands, horizontal scrolling registers, and others)
* Support for up to 19268 colours
* 13 Screen Modes
* Resolutions up to 512 x 212

This is the first board produced within the "Yellow MSX Series".

### What is the 'Yellow MSX Series of Boards'?

These are a series of boards that I am developing to achieve MSX compatibility for RC2014 systems.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

The V9958 board is the first in that series.

More details can be found on my [hackaday project](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

### Key features of this module

* Compatible with RC2014 (enhanced bus required)
* 128K of RAM
* RGB, Composite and S-Video output
* Can be built for PAL or NTSC configuration

### Revision 1.9 enhancements

* Addition of noise filtering for both the V9958 and CXA1645P chips
* Compatible with RC2014's operating with Interrupt Mode 2
* Replaced diode-gate chip select circuit with PLD gates - enabling more reliable operation.
* Selectable sync signal for the RGB output (TTL, composite or Luma)

### Alternative Video Modules

This kit has the CXA1645P chip, that enables the output of composite and s-video signals (PAL or NTSC).  An alterative kit is available that does not include the CXA1645P and associated components.  It produces the RGBs output via standard 15-pin DSUB/VGA connector.

If you plan to only use RGBs output, you may want to consider that alternative [RGB DSUB/VGA output](https://www.tindie.com/products/dinotron/v9958-msx-rgb-video-module-for-rc2014/) kit.

### Video conversion/display options

If you plan to connect this module to a modern LCD monitor, it would need to an input that supports one of: RGBs 15Khz, composite or s-video.

If your display does not support such inputs, a converter will be required.

The [RetroRGB upscalers article](https://www.retrorgb.com/upscalers.html) has a good description for some of the converter options.

Items such as retroTink and OSSC are very high quality converters - but also have a decent price tag.

I have found the low cost arcade converters such as the GBS-8200 and HD-VC9900 converters produce excellent results.  The upscalers can be acquired through the usual ebay, amazon and other sellers at very reasonable prices.

The GBS-8200 can also be easily modified to produce more customisable and higher quality output - again RetroRGB has it all explained at [GBS Control](https://www.retrorgb.com/gbs-control-installation-overview.html)

If you plan to use the GBS8200 or HD-VC9900 converters, then this kit is may be the better option.

Please note, that for the RGBs output, a custom cable will probably need to be built.

### What's included in this kit

The kit can be supplied with individually tested *V9958*, *4164C* and *CXA1645P* chips.  Or you can choose to exclude these chips if you wish to source them yourself.

Everything else you need is included (PCB, passive components, crystals, sockets, connectors and other ICs).

The full kits includes everything you need (PCB, passive components, crystals, sockets, connectors and other ICs)

The ATF16V8 is supplied new and programmed for immediate operation.

The components for both the PAL and NTSC versions are included.

### What else do I need to make this work?

* You need a working RC2014 system (cpu, RomWBW, Clock, etc - enhanced bus required).
* A solution to display Composite, S-VIDEO or RGBS signals.  For a modern monitor you will almost certainly need a converter of some kind.
* For RGB output (recommended for best quality), you will need to make a cable to connect to your preferred display or converter.

### Construction notes

Please note that due to the finer pitch pins of the V9958, the soldering of this board requires a little more finesse than the typical RC2014 through-hole based boards.  If you have built a few boards already, then you should be fine.  It's recommended that a finer pitch soldering tip be used as it can be easy to accidentally bridge some of the pins.

Extra care needed when inserting the V9958 into its socket - there are lots of pins, take your time to avoid bending pins.

You need to identify if you plan to build a PAL or NTSC version and select the correct resistor and capacitors for R13 and C14.

For more notes and any revisions please have a look at the [github page](https://github.com/vipoo/yellow-msx-series-for-rc2014/blob/main/video/README.md)

### Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with.  I will do my best to answer any support questions you may have.

The V9958, CXA1645P and 41464C chips are obsolete and out of manufacture.  They may be old-new-stock or pre-used, and as such there is a risk they may not function correctly.  Therefore, I fully test each chip, in-circuit, prior to packaging, so you can have good confidence that the chips you receive will perform as required.

I have conducted the following tests for this board:

- RGB connected to a [OSSC](https://www.retrorgb.com/ossc.html), GBS8200 and HD-VC9900 upscalers, to a modern LCD monitor.
- Composite and S-VIDEO output connected to a cheap ebay VGA converter (colours and quality output ok but not brilliant).

