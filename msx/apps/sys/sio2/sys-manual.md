
### SIO/2

```SERVICE=SIO2.SYS```

A serial driver for the RC2014 SIO/2 kit.

Port B only.

The SIO/2 only support 3 configurable BAUD rate setting.  The specific BAUD rate is a function of the clock supplied to the Port B serial interface (typically configured as 3.686400Mhz).

Assuming it's configured with this clock speed, the allowable BAUD rates are: 4800, 9600, 19200

Beta driver - Hardware flow control is locked on.  Protocol is locked at 8 bit 1 status, no parity.
