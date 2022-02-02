## Hackaday Project:

* [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

<a href="https://www.tindie.com/products/dinotron/???"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-smalls.png" alt="Available for purchase at Tindie" width="200" height="55"></a>

## Title

Turbo CPU Module Designed for RC2014

## Brief Description

Run a Z80 CPU at up to 20Mhz

### Description

A Z80 CPU module supporting speeds up to 20Mhz - but with the required wait states and clock control to ensure compatibility with existing software and RC2014 modules.

### What is it?

This module is designed to replace your existing RC2014 CPU and Clock Modules and give you the full power of a Z80 running at 20Mhz, yet still be broadly compatible with original software and your other RC2014 modules.

For a RC2014 configured to run a full MSX configuration, you typically need to run your CPU at just 3.5Mhz, as software written for the platform at the time, would assume your system was clocked at this speed.  If you attempt to run programs at a higher cpu clock speed, you will probably get video corruption, I/O problems -- it just wont work!.

But these days, its possible to buy a brand new Zilog Z80 chip, rated at 20Mhz (Z84C0020PEG).  For me in the mid 80s, that would have been an unimaginable speed!

We achieve compatibility by applying a combination of hardware wait states (pausing the CPU for a bit) and slowing CPU clock down to the 3.5Mhz as required.

With the 3 way slider, soldered on the front of the module, you can at any time switch the CPU into 1 of 3 modes:

1. Full 20Mhz clock speed, with 1 wait state for accessing memory, and auto slow the clock to 3.5Mhz for 31 clock cycles when it accesses IO devices (SIO/2, PPI, V9958 etc).
2. Full 20Mhz clock speed as mode 1, but with 3 wait states for accessing memory
3. Standard MSX speed 3.5Mhz CPU, M1 Wait state for accessing memory

Despite the extra wait states and clock slow down, I have found a typical speed improvement of between 4 and 5 times faster - even for software the does lots of interactions with the V9958, you can still see a very large improvement.

### Key features:

* Support Z80 running at 20Mhz (16Mhz oscillator also supplied if you have issue with your specific kit)
* 3 way slider to select fast, medium or standard speeds
* Blinky Leds to indicate turbo mode and clock slow down states
* PLD to control wait state counts and clock slow down - allowing for experimentation if you want to try other speeds configurations

### What is the 'Yellow MSX Series of Boards'?

These are a series of boards that I am developing to achieve MSX compatibility for RC2014 systems.

The idea is that you can build each board one at a time, test it and play with it under RomWBW - and then once you have the set - load up some MSX/MSX2+ games!

The V9958 board is the first in that series.

More details can be found on my [hackaday project](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

### What's included in this kit

The full kits includes everything you need (PCB, capacitors, IC sockets, connectors, and the ICs).  The 2 PLD chips are flashed with logic to control the wait and clock slow down logic.

Also included is a 20Mhz and an alternative 16Mhz oscillator.

### Operation Reliability

I have tested running at 20Mhz on my specific back-plane and set of modules without any problems.  But due to differences in combination and configuration of RC2014 kits - you may find with your specific situation, there are signal degradation issues at 20Mhz that may cause non-reliable operation.  With the supplied 16Mhz alternative oscillator, you should be able to achieve good operation, but at a slightly lower speed.

### Bill of Materials


### What else do I need to make this work?


### How does it work?

Running RC2014 build at higher clock speeds presents a few challenges for reliable operation. To increase the CPU clock speed of your system, a few things need to be considered:

1. Whats are the frequency and rise/fall time limits for the back-plane and various modules?  Its very easy to introduce cross talk or other signal quality issues as your clock rate goes up.
2. What the timing limits for the typical RAM/ROM modules. If you try to run your Z80 at 20Mhz, will the RAM and ROM modules keep up?
3. Similarly, whats the timing limits for various I/O devices. Some chips, like the SIO/2 might be rated only to speeds around 6Mhz. It is unlikely such chips will cope with a Z80 running at 20Mhz.
4. In addition devices like the V9958 Video Display Processor have additional timing requirements - the rate of 'commands' that can be sent and executed must not exceed its capacity. If you send drawing commands to this chip too quickly, you will likely get video corruption.

All the various I/O modules and RAM/ROM modules available for RC2014 typically expect an operation frequency of between 3.5Mhz and 7Mhz. So if we increase the clock frequency of the CPU, we have to figure out a way to ensure a wide range of compatibility with all these external modules.

One of the things that work in our favour though, is despite the fact the Z80 is running at 20Mhz, it will not access I/O and memory at this rate.  This is due to the way it requires a few internal clock cycles for various operations, so the effective rate of communications to modules will be lower. For example, all I/O request take at least 4 Clock cycles to complete, and some instructions processed by the Z80 can take up to 20 clock cycles.  We have to consider the timing requirements and operation of the Z80 as described in its datasheet.

The Z80 supports the concepts of additional hardware based wait states.  By designing some logic to enable extra wait states when accessing I/O and memory devices, we can force the CPU to 'wait' or pause for additional clock cycles, thus allowing the I/O and memory chips the time they need to process the read/write requests from the CPU.

It also important to remember, that even at the 3.5Mhz clock speed, the original MSX standard required an extra wait state when reading memory during the M1 cycle.  This was required as the speed of memory, particularly ROM chips of the day, were not able to keep pace with a Z80 running at 3.5Mhz.  Although modern RAM and ROM chips are quite capable of keeping up, if we remove this wait time, original software and games may not work right.

The V9958 in particular, presents a compatibility challenge when running the Z80 at a higher clock speed.  So much original software will be optimise to send commands to the V9958, on the assumption that the CPU is running at 3.5Mhz.  Although, with my testing to date, I found the V9958 can communicate with a Z80 running at 7Mhz.  This will only work as long as our software manages the rate at which it issues commands to the V9958.  Original unaltered software, typically will cause video corruption, even at 7Mhz.

The timing requirements for the V9958 can be quite complicated.  Depending on if you are issuing a command or attempting to access the VRAM the time thats needed between 1 operation and the next can be quite confusing to figure out.  Despite this, the software authors, have optimised this to the highest possible degree.  For a detailed analysis of the timing, check this [article](http://map.grauw.nl/articles/vdp-vram-timing/vdp-timing.html) out.  Any increase in CPU speed without hardware waiting, will cause original software to not work.  But if we go with 'pausing' the CPU to achieve compatibility, running at 20Mhz, we may need to pause the CPU for 90 clock cycles (or more).

So to solve this problem, instead of 'pausing' the CPU, I went for a different approach.  When I/O interactions are undertaken by the Z80, the Z80's clock is switched from the 20Mhz rate, down to the standard 3.5Mhz. The CPU is feed this slow clock for 31 clock cycles (at 3.5Mz), after which it revert the clock back to the full 20Mhz.  I found that about 30 odd cycles was enough to achieve general compatibility.




## Disclaimer

Please note that this is a kit, produced by a non-professional (me) for hackers, DIYers' and retro lovers, to tinker with. I will do my best to answer any support questions you may have.

## Images

Assembled
---------
<img src="images/installed.jpg" alt="Installed" width="100%"/>

<img src="images/assembled-profile.jpg" alt="Assembled Profiled" width="100%"/>

<img src="images/assembled-top.jpg" alt="Assembled Top" width="100%"/>

<img src="images/top.jpg" alt="PCB Top" width="100%"/>

<img src="images/back.jpg" alt="PCB Back" width="100%"/>

## References

Hackaday Project: [msx-compatible-boards-for-rc2014](https://hackaday.io/project/175574-msx-compatible-boards-for-rc2014)

Kits available at: https://www.tindie.com/products/dinotron/?????
