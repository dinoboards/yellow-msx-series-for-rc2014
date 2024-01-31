$define{doc_title}{Yellow MSX User Manual}$

<!-- https://github.com/Wandmalfarbe/pandoc-latex-template/blob/master/examples/title-page-logo/document.md
geometry:
- top=2cm
- bottom=3cm
- left=2.5cm
- right=2.0cm
toc-depth: 2 -->

---
title: Yellow MSX Manual
author: Dean Netherton
toc: false
toc-own-page: false
classoption:
- oneside
titlepage: true
titlepage-rule-color: "fcc603"
titlepage-rule-height: 4
titlepage-logo: "docs/dino.png"
logo-width: 120mm
fontsize: 13pt
papersize: A4
numbersections: true
secnumdepth: 2
colorlinks: true
geometry:
- top=4cm
- bottom=5cm
- left=3.0cm
- right=2.5cm
header-includes:
- |
  ```{=latex}
  \usepackage{awesomebox}
  \usepackage{wrapfig}
  \renewcommand{\linethickness}{10pt}
  \definecolor{abnote}{RGB}{255, 204, 0}
  \definecolor{yellow}{RGB}{255, 204, 0}
  \definecolor{red}{RGB}{208, 72, 72}
  \definecolor{orange}{RGB}{243, 185, 95}
  \definecolor{blue}{RGB}{104, 149, 210}
  ```
pandoc-latex-environment:
  noteblock: [note]
  tipblock: [tip]
  warningblock: [warning]
  cautionblock: [caution]
  importantblock: [important]
...

# Preface {.unlisted .unnumbered}

\begin{wrapfigure}{r}{0.40\textwidth}
  \centering
  \includegraphics[width=0.40\textwidth]{docs/full-kit-profile-1.jpg}
\end{wrapfigure}


So you have finished building and assembling your MSX kits and you've put the soldering iron way.  You have a working MSX compatible computer built with your very own hands! Congratulations and well done - that's an achievement you certainly can be proud of.

But now what, you ask?  Perhaps you have typed a few commands at the DOS prompt.  Perhaps you figured out how to load a game or write a little BASIC program and run.  But you want to explore and learn more.  What can you make this little machine do?

Well this manual is here to help.  Whether you have lots of previous experience with 8 bit computers from the 80s or not, this guide will help you in your continuing exploration of all things 8 bit retro.

\small
\begin{noteblock}
\begin{raggedright}

The Yellow MSX is a retro self assembled computer kit.  Its made up of a number of modules (CPU, Memory, Storage, Sound, Keyboard and Video), that once assembled together into a working computer - give you a fully operational MSX compatible computer.

Due to its modular nature, you can customise and extend the system in many ways, including installation of other RC2014 compatible modules.

More details can be found on the official GitHub page:

https://github.com/vipoo/yellow-msx-series-for-rc2014/tree/main?tab=readme-ov-file\#yellow-msx-series-for-rc2014
\end{raggedright}
\end{noteblock}
\normalsize

\pagebreak

# Glossary {.unlisted .unnumbered}

|Term|Meaning|
|-|------|
|8-bit|Refers to the computer's central processing unit (CPU) architecture, where each data unit is 8 bits long.|
|Assembler|A program that converts assembly language code into machine code for the computer's CPU.|
|BIOS|Basic Input/Output System, a set of software instructions stored in ROM that initializes hardware during the boot process.|
|Cartridge|External plug-in modules containing additional software or hardware enhancements for the computer.|
|CLI|Command Line Interface - A text-based interface where users interact with the computer by entering commands.|
|CPU|Central Processing Unit, the primary component responsible for executing instructions in the computer.|
|I/O|Input/Output - Refers to the communication between the computer and external devices.|
|MSX-BASIC|The built-in programming language for MSX computers, providing a simple way to write and run programs.|
|MSX™|A standardized home computer architecture developed by Microsoft and ASCII Corporation in the early 1980s.|
|Palette|A set of colors available for use in graphics and display.|
|Pixel|Short for picture element, the smallest unit of display on a computer screen.|
|RAM|Random Access Memory, the volatile memory used by the computer for temporary storage of data and program code.|
|ROM|Read-Only Memory, a type of non-volatile memory that stores the computer's firmware or permanent software.|
|Sprite|A graphical object that can be moved independently on the screen, commonly used in 2D gaming graphics.|
|VDP|Video Display Processor, a dedicated chip responsible for handling graphics and video output.|
|RC2014™|A simple 8 bit Z80 based modular computer kit system (see https://rc2014.co.uk/)|

\small
\begin{cautionblock}
\begin{raggedright}
The term 'RC2014' is a registered trademark of RFC2795 Ltd. The Yellow MSX kits are not affiliated, authorized or endorsed by RFC2795 Ltd.

All other trademarks mentioned in this document are the property of their respective owners and any use of these trademarks also does not imply affiliation, authorisation or endorsement.
\end{raggedright}
\end{cautionblock}
\normalsize

\newpage
\hypersetup{linkcolor=}
\tableofcontents


# System Overview

## Official Yellow MSX Modules

Below is a list of all the MSX Kit Modules available.

|Module|Description|MSX|RomWBW|
|--|------|-|-|
|12+1 Backplane|Support up to 12 Modules, and Cartridge Slot Extension|Yes|Yes|
|Cartridge Slot|Add support for 2 MSX Cartridges|Optional|No|
|Cassette+USB|Compatible cassette interface and a USB Port|Optional|No|
|Game|Add basic sound and game controller inputs|Yes|Yes|
|Keyboard|73 Key MSX keyboard|Yes|Yes|
|Memory|1024K RAM/512K ROM Module|Yes|No|
|Music|Extended Music Support (YM2413)|Optional|Yes|
|PPI|Keyboard interface &  Memory Slot signals|Yes|No|
|RTC|Real time clock|Optional|Yes|
|Turbo CPU|Z80 CPU operating from 3.5Mhz to 20Mhz|Yes|Yes|
|V99x8 MSX RGB|MSX RGB video output|Yes|Yes|

\begin{tipblock}
\small

It is possible to use the standard RC2014 Backplane Pro for some modules, but as not all lanes are supplied, some additional wiring will be required. The Cartridge Slot Extension, though is only compatible with the 12+1 Backplane.

\end{tipblock}
\normalsize

## Compatible RC2014 Modules

Some of the official and 3rd-party RC2014 Modules will work in a system configured for MSX operation.  These includes:

|Module|Notes|
|-|--|
|Z80 CPU v2.1 & Dual Clock|Clock must be configured at 3.5Mhz|
|Compact Flash|Must be formatted for MSX (see FDISK utility)|
|Dual Serial SIO/2|Only the B Port supported|
|Bubble LED Display|Controlled through MSX Basic|
|Others|So long as there are no port conflicts there is a good chance it will work in a MSX configured kit|

\pagebreak

## System ROM

The Memory Module, includes a 512K boot ROM.  On powerup and/or reset, the system will execute the code flashed within. The ROM image contains numerous sub-systems for the operations of the computer - and there are numerous pre-built images available for you to download to suit your requirements.

The ROM images available for download are for the most part, identical.  The key differences are the choice of system BIOS/MSX-BASIC and country/date formatting rules.

The following table describes the key subsystem that the various system ROMs contain.

|Unit|Description|
|-|--|
|SYSTEM BIOS|Manages the bootup sequence and provides basic I/O access for common hardware devices (keyboard, memory paging, video).|
|MSX-DOS/NEXTOR KERNEL|The core kernel of disk operating system built for the MSX platform|
|DISK DRIVERS|Disk drivers written for this platform (RC2014 Compact Flash, USB storages and others)|
|EXTENDED BIOS|Additional hardware support for the platform (SIO/2 and USB)|
|EMBEDDED BOOT DISK|A small read only disk image containing MSX-DOS boot files and other utilities, to enable the platform to boot into MSX-DOS, when no external storage devices are detected|
|MSX-BASIC|A specific country configured build of MSX BASIC|

There are 2 core image types that can be installed on your system:

1. CBIOS Basic system, configured for 50Hz or 60Hz video display rate
2. MSX-BASIC system, configured for specific country and format defaults

### CBIOS images

The CBIOS image includes all units, except for MSX-BASIC.  It supports loading and running the vast majority of MSX-DOS programs and games.

As it does not include MSX-BASIC, some programs and all BASIC programs will not be work.

You can find more information of CBIOS at their website: https://cbios.sourceforge.net/

There are 2 variations of the CBIOS image. One for 60Hz video displays and the other for 50Hz. Both of these images will work if your output is RGB.  If you are using composite or S-Video output, you will need to ensure the correct frequency for your display type (eg: 50Hz for PAL, 60Hz for NTSC).

### MSX-BASIC images

These are the recommended images, as they enable most compatibility. With one of these images, you should be able to run all MSX-DOS, GAMES and BASIC programs.

At time of writing an image with the following configurations are available:

|Country|Frequency|Char Set|Date Format|
|-|-|-|-|
|Australia|50Hz|International|D-M-Y|
|Japan|60Hz|Japanese|Y-M-D|
|USA|60Hz|International|M-D-Y|
|international|50Hz|International|M-D-Y|
|UK|50Hz|International|D-M-Y|
|France|50Hz|International|D-M-Y|
|Germany|50Hz|International|D-M-Y|
|Italy|50Hz|International|D-M-Y|
|Spain|50Hz|International|M-D-Y|
|Arabic|60Hz|Japanese|Y-M-D|
|Korean|60Hz|Japanese|Y-M-D|

# RC2014 Extensions

The rom images mentioned above include bios extensions (using the MSX EXTBIO interface), to
provide application services specific to the platform.

These allow control of items such as the RC2014 SIO/2 module
and interacting/enumerating USB devices attached to the Cassette+USB module.

See [docs/extended-bios.md](docs/extended-bios.md) for details of the specific enhancements

## Compact Flash

The embedded ROM NEXTOR driver will detect and allow the use of the RC2014 Compact flash modules.

The use of `FDISK` is probably required to initialise the disk with FAT12/FAT16 partitions.

## Embedded Boot Disk

On boot, if no disks devices are found containing a bootable MSX-DOS/NEXTOR image, then the system
will boot from a read-only disk image included in the ROM.

If you do have a compatible disk images (USB or CF), you can use `FDISK.COM` on the embedded boot image to prepare your desired boot device, and then copy the `NEXTOR.SYS` and `COMMAND2.COM` (and optionally the `AUTOEXEC.BAT`) files
from this embedded image to the device.  When you reboot, the system should boot from attached device in preference to the embedded ROM disk image.

# Applications & Services

## System Services and Drivers

As the Yellow MSX system can support many different hardware extensions, appropriate driver software is required.  As there is limited space within the 512K ROM to store all the drivers required for all the potential hardware installed on your system, a dynamic service framework was developed.

This framework is similar to the MSX [MemMan](https://www.msx.org/wiki/MemMan) system.

> (MemMan has an unknown compatibility issue, and as at writing has not been resolved.)

This service framework, allows the installation of TSR (Terminate Stay Resident) software.  The services are copied into a free segment of 16K page, thus do not consume any of the TPA that is available for standard MSX-DOS and Basic programs.

All service TSR programs have the extension `.SYS`, eg: `PRNT.SYS`

To installed a service, you can use the `SERVICE.COM` application to load them. For example to install the USB printer driver, you can run the following command.

```
SERVICE=PRNT.SYS
```

This command can be placed into your AUTOEXEC.BAT script, so that the printer driver is always loaded at system start.

## Services

$include{"../bin/sys-manuals.md"}$

## Yellow MSX specific apps

The Yellow MSX for RC2014 in addition to all the standard MSX-DOS applications, includes some programs specifically designed for the system and its hardware modules.

$include{"../bin/app-manuals.md"}$
