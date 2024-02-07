
### MODE

Program to change various system settings.

Supported sub commands:

|Sub System|Command|
|--|---|
|Serial port:   |MODE COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s]|
|Display width: |MODE \<cols>|

#### Serial Driver

An appropriate serial driver needs to have been installed.  See the USB FTDI or SIO/2 system drivers.

`MODE COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s]`

|Parameter|Description|
|-|-----|
|COMm[:]|Specifies the number of the communications port. eg MODE COM2: ...|
|BAUD=b|Specifies the transmission rate in bits per second. No check is conducted to verify if the specific driver/hardware supports the supplied baud rate.  eg: MODE COM2: BAUD=19200|
|PARITY=p|Specifies how the system uses the parity bit to check for transmission errors. The p value can be one of the following: N (none), E (even), O (odd), M (mark), or S (space). The default value is E. Not all computers support the values M and S.|
|DATA=d|Specifies the number of data bits in a character. Valid values for d are in the range 5 through 8. The default value is 8. Not all computers support the values 5 and 6.|
|STOP=s|Specifies the number of stop bits that define the end of a character: 1, 1.5, or 2. If the baud rate is 110, the default value is 2; otherwise, the default value is 1. Not all computers support the value 1.5.|

#### Display width

Changes the number of characters per line on the screen in text mode.

The screen will be cleared and the cursor moved to the top left corner in the process.

|Parameter|Description|
|-|-----|
|\<cols>|Number between 1 and 80 to specify the number of characters per line you want to have in text mode.|

> If your system has the RTC/F4 Module, then the new applied width will be preserved across system reboots.  Otherwise, if you do not have the RTC/F4 module, on reboot the system will default back to a 40 column width.

