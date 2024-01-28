### TERM

A simple ANSI compliant telnet console.  For use with a serial connected device.

Requires a compatible serial device driver to be installed (USB FTDI or SIO/2).

Currently only support accessing the first installed serial device (COM1).

`TERM [/a] [/o] [/r] [/i] [/b\<rate>]`

|Parameter|Description|
|---|---|
|/a|turn off automatic download detection|
|/o|turn off ANSI rendering and use MSX-DOS text rendering|
|/r|Use alternative download method|
|/i|Display information and instructions|
|/b\<rate>|select baud rate.  Must be supported by the current driver|
