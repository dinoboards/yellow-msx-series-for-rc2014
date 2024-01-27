## RTCCALB

Utility to aid in calibrating the RTC module, using the CPU Clock as a reference.

> Not compatible with TurboCPU mode.  If using the Turbo CPU module, make sure you have selected the standard speed (3.68Mhz)

 `RTCCALB [/cpu=\<clock>] [/m1]`

|Parameter|Description|
|---|---|
|/cpu=\<clock>|The cpu clock 3686400|
|/m1|If your CPU module adheres to the MSX standard for wait states (RC2014 CPU does not, the MSX Turbo CPU in standard speed does)|

