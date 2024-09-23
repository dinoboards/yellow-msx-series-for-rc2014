## Hackaday Project:

* [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

<a href="https://www.tindie.com/products/edit/msx-cassette-usb-module-designed-for-rc2014"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-smalls.png" alt="Available for purchase at Tindie" width="200" height="55"></a>

## MSX Cassette + USB Module Designed for RC2014

<img src="images/pcb-profile-usb-top.jpg" alt="Assembled Profiled" width="50%"/>

## Brief Description

Save your MSX-BASIC programs to cassette tape and give your Z80 access to a wide range of USB devices.

### Description

Relive the olden days of storing and loading programs onto cassette tapes.  Experience the nostalgia of the old medium or perhaps find out what all the fuss was about.

But if that is not enough for you, then use the other side of this kit and bring your MSX RC2014 build into the 90's with Universal Serial Bus.  After updating your ROM images, you will be able to load and store via MSX-BASIC or MSX-DOS (NEXTOR) on:

* USB flash devices
* USB magnetic mechanical hard disks
* USB 3.5" Floppy drives
* and probably many other USB storage devices.

But that's not all, you can also:
* use compatible USB hubs to attach multiple devices (supports up to 4 storage devices)
* with the use of a USB to Centronics adapter, connect your old dot matrix printer - (earplugs are also recommended)

There are also plans to update the software to support keyboard, ethernet adapters, and other useful USB devices.

### What is it?

As described above this kit has 2 key features.

* A true retro cassette interface to save and load programs on an external regular cassette recorder (or any device capable of recording and playing back audio).
* An only slightly less retro USB 1.1 port which can enable your build to connect and utilise an assortment of USB 1.1/2.0 devices.

The cassette interface uses a conventional MSX design based around the LM311N Voltage Comparator.  This little circuit will convert the 'analogue' signal from the cassette to a digital TTL signal

The USB interface use the cheap CH376S module.  A module that handles the low level 12Mhz signals.  The Z80 is still responsible for all the USB protocol encoding/decoding for various USB device classes - such as UFI for floppy and SCSI for Mass storage.

> The CH376 does provide a high level interface for connecting to most common mass storage, but is limited and will not work through hubs.  The MSX-DOS (NEXTOR) driver written does not use this feature of the chip, and instead uses the lower level USB protocol to implements the specific USB class for the attached storage device and will work though hubs, and support a wider range of devices.

### Key features:
* MSX Cassette storage interface (with motor signal LED)
* USB 1.1/2.0 compatible (although certainly not certified)
* Tested with a large assortment of flash drives, HDD, Floppy, printer, and hubs.
* Drivers written in C so a little more accessible than pure Z80 assembly
* Individually programmable LEDs to indicate USB operation

### Images

<table>
  <tr>
    <td width="50%"><a href="./images/pcb-assembled-top.jpg"><img src="./images/pcb-assembled-top.jpg" width="90%"/></a></td>
    <td width="50%"><a href="./images/pcb-profile-cassette.jpg"><img src="./images/pcb-profile-usb-top.jpg" width="90%"/></a></td>
  </tr>
  <tr>
    <td width="50%"><a href=""><img src="./images/pcb-top.jpg" width="90%"/></a></td>
    <td width="50%"><a href=""><img src="./images/pcb-bottom.jpg" width="90%"/></a></td>
  </tr>
  <tr>
    <td width="50%"><a href=""><img src="./images/pcb-profile-cassette.jpg" width="90%"/></a></td>
    <td width="50%"><a href=""><img src="./images/pcb-profile-usb-bottom.jpg" width="90%"/></a></td>
  </tr>
</table>


### Testing Status

The correct interfacing with any USB device is mostly limited by the software drivers that have been specifically written to enable appropriate support under MSX-DOS/NEXTOR.  So any limitation or issue can probably, once identified, be corrected with a software update.

There has certainly not been any USB certification conducted.  Just lots of manual testing by me.  There are possibly still things in the software that are not following the rules.

Under MSX-DOS/NEXTOR hot swapping of devices is not supported.  Insert your USB devices then power on or reset.  You can change floppies, but do not unplug and re-plug the drive.

There are 4 main USB classes implemented to date.  The UFI which is used by floppy drives, SCSI which is used by flash, HDD and other large storage devices, the printer class for Centronics parallel printer adapters and enumeration through USB hubs.

#### **SCSI: Flash/Thumb and HDD drives:**

<table>
<tr>
<td width="80%">

* I have tested against 9 different Thumb/flash drives of varying sizes.  Found and resolved all issues.
* Tested against a couple of external magnetic hard disk drives.
* Formatting of old magnetic drives not supported. (Partitioning through NEXTOR works as required though)

</td>
<td>
<img src="./images/flash-drives.jpg" height="100px" />
</td>
</tr>
</table>

#### **UFI: Floppy drives:**

* I have tested against 3 USB 3 1/2" floppy drives.  They each operated slightly differently. For the most part they all work.
* I did experience some specific compatibility issues with these drives - but so did windows, so I suspect its more limitations of the specific drives.


<br/>
<br/>

<table>
<tr>
<td width="60%">

**TEAC drive - purchased on ebay.  Seems to be the most common available today**

* Recommended
* Can format, read and write to 720K and 1.44MB
</td>
<td>
<img src="./images/teac-top.jpg" height="100px" />
</td>
<td>
<img src="./images/teac-back.jpg" height="100px" />
</td>
</tr>
<tr>

<td width="60%">

**IBM drive - Seems to be an 'older' TEAC drive internally**

* can read and write 720K and 1.44Mb disk
* formatting was unreliable - 720K formatted does not seem to be supported

</td>
<td>
<img src="./images/ibm-top.jpg" height="100px" />
</td>
<td>
<img src="./images/ibm-back.jpg" height="100px" />
</td>
</tr>

<td width="60%">

**Imation drive - Old drive**

* read/write 720K disk ok
* does not support HD 1.44MB
* formatting not tested
</td>
<td>
<img src="./images/imation-top.jpg" height="100px" />
</td>
<td>
<img src="./images/imation-back.jpg" height="100px" />
</td>
</tr>
</table>

#### **USD Hub:**

All but one of the hubs tested works.  All but one of the hub seemed to have been 'certified'.

<table>
<tr>
<td width="60%">

**Orico 4 Port USB 3.0 Transparent Hub**

* Highly Recommended
* externally powered which is really recommend if using floppy disks

</td>
<td>
<img src="./images/orico-hub.jpg" height="100px" />
</td>
</tr>
<tr>

<td width="60%">

**Generic USB hubs**

* all tested ok

<img src="./images/cheap-hub-1.jpg" height="100px" />
<img src="./images/cheap-hub-2.jpg" height="100px" />
<img src="./images/cheap-hub-3.jpg" height="100px" />
</td>
</tr>

</table>

#### **Printer adapters:**

* Tested with 2 USB to Centronics adapters from ebay.  Both seem to use the same chips internally.

* Tested with one 9pin dot matrix printer.

<table>
<tr>
<td width="60%">
<img src="./images/printer-1.jpg" height="100px" />
<img src="./images/printer-2.jpg" height="100px" />
</td>
</tr>

</table>

> #### Printer driver (ROM builds after 2023-11-06)
> For ROM builds after 2023-11-06, the installation of a USB printer driver is required.  (The driver is no longer included in the embedded ROM image)
>
> The are 2 executables needed to install the driver (`SERVICE.COM` and `PRNT.SYS`).
> In your AUTOEXEC.BAT file, you need to add this section:
>
> ```
> SERVICE=PRNT.SYS
> ```
> If no printer or USB is detected during the install - then no driver will be installed.  If installation is successful, you will see the following output:
>
> ```
> PRNT.SYS:    INSTALLED
> ```


### What is the 'Yellow MSX Series of Boards'?

These are a series of boards developed to achieve MSX+ compatibility.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

The V9958 board was the first in that series.

### What's included in this kit

The full kits includes everything you need (PCB, capacitors, IC sockets, CH376S module, connectors, and the ICs).  Also includes a small Flash drive that's been tested and confirmed to work with the current software revision.

### Bill of Materials

|Count   | Name  |
|:------:|-------|
|  3     |	0.1uF Ceramic Capacitors  |
|  2     |	22nF Ceramic Capacitors   |
|  1     |	1uF  Electrolytic Capacitors |
|  1     |	10uF  Electrolytic Capacitors  |
|  1     |	220uF Electrolytic Capacitors  |
|  2     |	1N4148 Diode   |
|  3     |	5mm LED   |
|  2     |	3.5 AUDIO SOCKET    |
|  2     |	10K Ω Resistor  |
|  2     |	1M Ω Resistor    |
|  1     |	169K Ω Resistor    |
|  6     |	4K7 Ω Resistor   |
|  1     |	47K Ω Resistor   |
|  1     |	1K Ω Resistor    |
|  1     |	100 Ω Resistor   |
|  3     |	470 Ω Resistor   |
|  1     |	CH376S Module   |
|  1     |	74HCT138    |
|  1     |	74HC74    |
|  1     |	74HCT32   |
|  1     |	LM311N    |
|  1     |	Right Angle 20x2 Header   |
|  1     |	Right Angle 1x20 Header   |
|  1     |	8 POS IC SOCKET   |
|  2     |	14 POS IC SOCKET    |
|  1     |	16 POS IC SOCKET    |
|  1     |	512MB USB Flash Drive   |

### What else do I need to make this work?

* A working MSX system on RC2014 bus
* An updated system ROM for the [MSX Memory Board for RC2014](../memory/README.md)

> You need a ROM image released on or after `2023-10-20`.  You can find prebuilt images under the [project's github releases.](https://github.com/dinoboards/yellow-msx-series-for-rc2014/releases)

> The cassette part of the module, as it requires specific MSX signals will only work in conjunction with the full MSX on RC2014 kits.

> The USB module and associated signal LEDS could in theory have the potential to work on a stock RC2014 - at this stage it will not.  There has been no software developed for this configuration, nor any testing done to confirm no conflicts or other unknown issues.

### Port Mapping

#### The board use the following IO addresses:

| Port |	Description|
|------|-------------|
| $88	(r/w) | CH376 data port  |
| $89	(r/w) | CH376 command port  |
| $8A (w) | LED control (bit 0 and 1 only) |
| $8B (w) | mirror of $8A |


#### The cassette singles are routed and controlled via other modules:

| Port |	Description|
|------|-------------|
| $AA (w) | Cassette output*<br/>bit 4 = Data recorder motor (reset to turn on)<br/>bit 5 = Set to write on tape<br/>|
| $A0-$A3 | Cassette input via the YM2149 sound chip

\* The cassette output (single and motor) are via the PPI chip, part of the keyboard controller

> Unless you have the PPI module and YM2143 Game module, the cassette interface will not work.

#### User bus pin mapping:

The 3 cassette signals form the other modules are sent via the 'USR' lines of the backplane
| PIN | Description |
|------|-------------|
| USR5 | Cassette In signal from MSX Game Module |
| USR8 | Cassette out signal from the PPI module (keyboard) |
| USR4 | Cassette motor (LED Only) from the PPI module (keyboard) |
|


### Accessing storage USB devices.

On boot up, the ROM code will auto detect the presence of the CH376 module, and mount any formatted/partition drives it find.

For flash/hdd drives, it will only auto mount the first partition of discovered storage devices - assuming the drive has FAT12 or FAT16 partitions.  (You can use the NEXTOR utilities `MAPDRV` to mount other partitions if desired)

For floppy, it will attempt to discover and mount the inserted floppy's disc.  Floppy disks are always first to mount (eg: from A:)

If your flash/hdd drive is not partitioned or does not have FAT12/FAT16 partitions, you will need to manually partition the disk.  You can use the `FDISK.COM` utility (included in the embedded boot rom)

> If you have any issues with `FDISK` or the drivers detecting your USB devices, you may find it helpful to use a PC to 'format' the image with FAT32 -- avoid the use of extended partition etc.

For floppy disks, no partition is required, but the disks do need to be formatted.  Most 3.5" disk are typically formatted with IBM FAT12 - and should work just fine as is.

But if you do need to format a floppy, you can use the `USBCTRL.COM` utility from within msxdos's prompt:

```
usbctrl floppy format a:
```

`USBCTRL` should detect the appropriate density of inserted disk (HD or DD) and issue the format commands to the drive.

### Accessing a USB printer.

The system will on boot, report if it has detected the presence of a USB to Centronics printer adapter.  Assuming you have connected the adapter to a simple dot matrix or compatible printer, you can use the standard MSX-DOS commands/process to send text to the printer.

For example, the following MSX-DOS command will send the file, `MYTEXT.TXT`, file to the printer.

```
COPY MYTEXT.TXT PRN
```


## Resources

* Schematic: [schematic](./schematic.pdf)
* Datasheet: [LM311N Voltage Comparator](./datasheets/lm311-n.pdf)
* Datasheet: [CH376](./datasheets/CH376DS1.PDF)

## References

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)
