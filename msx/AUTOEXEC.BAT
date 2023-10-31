RAMDISK 300
SET PATH=%_BOOT%:\;%_BOOT%:\SYSTEM\;%_BOOT%:\APPS\
SET EXPERT=ON
SET TEMP=H:\

REM Tell the SIO driver the clock the SIO/2 chip
REM is configured to.
SIOCFG CLK=307200

service=%_BOOT%:\SYSTEM\prnt.sys
