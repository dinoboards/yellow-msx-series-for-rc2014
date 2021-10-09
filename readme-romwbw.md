# RomWBW Configuration

The [apps-rc2014](./apps-rc2014) directory contains source code to test and explorer the functionality of these boards under a RomWBW system.

* dots.com - test the V9958 video board by drawing a series random dots
* lines.com - test the V9958 video board by drawing a series random lines
* mbrot.com - test the V9958 video board by slowly drawing a mandelbrot set
* control.com - test the GAME board by writing to serial out, as buttons are pressed on any attached joystick/game pads
* tune.com - copied from [RomWBW project](https://github.com/wwarthen/RomWBW). Play chiptune files on the GAME board - see the tunes directory for samples

## Building RomWBW apps

to build the RomWBW test applications:

```
cd apps-rc2014
make
```

[Back](./README.md)
