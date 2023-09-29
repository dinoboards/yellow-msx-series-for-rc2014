
## V9958 Video Modules Designed for RC2014

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

<a href="https://www.tindie.com/products/dinotron/v9958-msx-rgb-video-module-for-rc2014"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-smalls.png" alt="Available for purchase at Tindie" width="200" height="55"></a>

### Description

A complete kit to give your RC2014 the advanced graphic capabilities of the mid to late 80s. It's based on the V9958 Video Display Processor (VDP) which powered
the MSX2+ line of 8 bit Z80 computers.

This V9958 chip, was produced by Yamaha in the late 80s and was an update to the their V9938 chip, which was itself, a successor to TI's TMS9918.  The V9938/V9958 addressed many of the shortcomings of the original TMS9918.

### Key features of this module

* Compatible with RC2014 (enhanced bus required)
* 128K of RAM
* RGBs via 15-pin DSUB/VGA connector
* Easy prototyping to various V9958 pins such as the colour bus, clock and other syncing signals.

<table>
  <tr>
    <td><a href="pictures/rgb-pcb-front-v2.2.4.jpg"><img src="./pictures/rgb-pcb-front-v2.2.4.jpg" width="100%"/></a></td>
    <td><a href="pictures/rgb-pcb-rear-v2.2.4.jpg"><img src="./pictures/rgb-pcb-rear-v2.2.4.jpg" width="88%"/></a></td>
  </tr>
  <tr>
    <td><a href="pictures/video-rgb-v2.2.4.jpg"><img src="./pictures/video-rgb-v2.2.4.jpg" width="100%"/></a>V2.2.4</td>
    <td><a href="pictures/video-rgb-profile.jpg"><img src="./pictures/video-rgb-profile.jpg" width="100%"/></a></td>
  </tr>
  <tr>
    <td><a href="pictures/rgb-top-with-low-esr.jpg"><img src="./pictures/rgb-top-with-low-esr.jpg" width="100%"/></a></td>
    <td><a href="pictures/rgb-profile-with-low-esr.jpg"><img src="./pictures/rgb-profile-with-low-esr.jpg" width="100%"/></a></td>
  </tr>
</table>

### DSUB pin out

<table>
  <tr>
    <th>Pin</th><th>Signal</th>
  </tr>
  <tr><td>1</td><td>Red    </td></tr>
  <tr><td>2</td><td>Green  </td></tr>
  <tr><td>3</td><td>Blue   </td></tr>
  <tr><td>4</td><td>NC     </td></tr>
  <tr><td>5-8, 10-11</td><td>GND     </td></tr>
  <tr><td>9, 12</td><td>NC   </td></tr>
  <tr><td>13</td><td>Sync**  </td></tr>
  <tr><td>14,15</td><td>NC    </td></tr>

</table>

\* The sync provided is as per the selection of J5.

To select TTL level for sync - apply a jumper to J5's top two pins

To select standard 1vpp sync signal - apply the jumper to J5 bottom two pins.

> **Please note that using a TTL level sync signal to device not designed for this level of voltage, may damage the monitor/converter's components.**

### Bill of Materials

|Count   | Name  |
|:------:|-------|
| 1      |  47pF  |
| 8     |   0.1uF |
| 5      |  10uF   |
| 4      |  220uF   |
| 2      |  22pF  |
| 1      | HDR 1x3 |
| 1      | SHUNT 1x2 |
| 1      |  5.6uH   |
| 1      |  15-PIN DSUB  |
| 3      |  2N3906  |
| 1      |  2N3904  |
| 3      |  10K  |
| 3      |  12K  |
| 3      |  27K  |
| 4      |  1K   |
| 1      |  470  |
| 8      |  100   |
| 1      |  V9958   |
| 1      |  ATF16V8  |
| 4      |  41464C  |
| 1      |  21.47727MHz  |
| 1      | Right Angle 20x2 Header |
| 1      | Right Angle header 1x20 |
| 4      |  18 POS IC SOCKET    |
| 1      |  20 POS IC SOCKET    |
| 1      |  64 POS IC SOCKET    |


### Output connections

You will want to think about how you plan to connect this board to your chosen monitor.  The video signals produced by 80's hardware is not trivial to connect to modern HDMI LCD monitors.  For such monitors you will need a converter to upscale the output.  Although VGA Monitors can accept a RGB signal, they are unlikely to support the lower frequency of 15Khz produced by these modules.

The RGBs module provides connection via the 15-pin DSUB/VGA connector.  This output is well suited to connect via a VGA cable to a compatible HDMI/VGA up-converter that accepts a RGBs signal over the 15pin connection, such as the OSSC, GBS8200 or HD-VC9900 converters.  For more information on the DSUB RGB/VGA see [RetroRGB's VGA article](https://www.retrorgb.com/vgaconnector.html)

The [RetroRGB upscalers article](https://www.retrorgb.com/upscalers.html) has a good description for some of the converter options.

Items such as retroTink and OSSC are very high quality converters - but also have a decent price tag.

I have found the low cost arcade converters such as the GBS-8200 and HD-VC9900 converters produce excellent results.  The upscalers can be acquired through the usual ebay, amazon and other sellers at very reasonable prices.

The GBS-8200 can also be easily modified to produce more customisable and higher quality output - again RetroRGB has it all explained at [GBS Control](https://www.retrorgb.com/gbs-control-installation-overview.html)


## Sample Apps

For a base ROMWBW build you can find compatible demo apps in the [apps-rc2014](../apps-rc2014) directory - they can be run under CP/M on a RC2014 system.  The prebuilt binaries can be found in the [bin](../bin) directory.

For a MSX-DOS system, binaries for various apps and demos can be found in the [github releases](https://github.com/vipoo/yellow-msx-series-for-rc2014/releases)

## Port Mapping

The board uses the standard IO addresses for MSX systems.

The first 2 ports are identical as per the TMS9918 chip.  The V9938/58 have additional ports to access higher functions.

| Port |	Description|
|------|-------------|
| $98	(r/w) | VRAM data  |
| $99 (w)	| VDP register selection |
| $99 (r)	 | Status register |
| $9A |	Palette access  |
| $9B	| Indirect register access  |

## Programming the chip

Have a look at the sample apps included in this repo [apps-rc2014](../apps-rc2014).

For specific details on programming the chip, I recommend:

* [V9938 Programmers Guide](http://rs.gr8bit.ru/Documentation/V9938-programmers-guide.pdf) hosted on (http://www.gr8bit.ru/gr8bit-knowledge-base.html)
* [V9938 Datasheet](../datasheets/yamaha_v9938.pdf)
* [V9958 Datasheet](../datasheets/yamaha_v9958.pdf)

* Also worth checking out some MSX VDP specific articles at: [http://map.grauw.nl/](http://map.grauw.nl/articles/).

## Resources

* Schematic (revision 2.2): [schematic-video-rgb-v2.2.pdf](./schematic-video-rgb-v2.2.pdf "Schematic (revision 2.2")


## Construction notes

Please note that due to the finer pitch pins of the V9958, the soldering of this board requires a little more finesse than the typical RC2014 through-hole based boards.  If you have built a few boards already, then you should be fine.  It's recommended that a finer pitch soldering tip be used as it can be easy to accidentally bridge some of the pins.

Extra care needed when inserting the V9958 into its socket - there are lots of pins, take your time to avoid bending pins.

The pads for the transistor footprints are very close together - take extra care not to bridge the pins.  Its recommend you solder the inner pins first.  After soldering each transistor, double check with a multimeter that there are no unexpected shorts.

### Revisions

* V2.2.3 - First revision sold on Tindie
* V2.2.4 - Minor changes to layout to allow for larger capacitors to be installed

### Errata for V2.2.3

The Diodes D1 and D2 are not required.  Solder a wire where the silkscreen has the diodes layed out (bottom right of pcb).

<img src="./pictures/video-RGB-assembled.jpg" width="50%"/>


## License
Copyright 2021 Dean Netherton

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

