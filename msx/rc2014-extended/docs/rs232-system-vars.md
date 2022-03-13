https://www.msx.org/wiki/System_variables_and_work_area#RS-232C


RS-232C
Data Area Used By RS-232C

| Address | Name      | Length | Description                                            |
|---------|-----------|--------| -------------------------------------------------------|
| FB03h   | RS_TMP    |  1     | Temporary data storage for RS232 Driver                |
| FB03h   | RS_TOCNT  |  1     | Counter used by the RS-232C interface                  |
| FB04h   | RS_FCB    |  2     | FCB ("File Control Block") address of the RS-232C      |
| FB06h   | RS_IQLN   |  1     | Byte DATA used by the RS-232C interface                |
| FB07h   | RS_MEXBIh |  5     | Hook called by the RS-232C                             |
|         |           |        | RS_MEXBIh = RST 30h (0F7h)                             |
|         |           |        | RS_MEXBIh+1 = Slot ID                                  |
|         |           |        | RS_MEXBIh+2 = Address                                  |
|         |           |        | RS_MEXBIh+4 = RET (0C9h)                               |
| FB0Ch   | RS_OLDSTT |  5     | Hook called by the RS-232C                             |
|         |           |        | RS_OLDSTT = RST 30h (0F7h)                             |
|         |           |        | RS_OLDSTT+1 = Slot ID                                  |
|         |           |        | RS_OLDSTT+2 = Address                                  |
|         |           |        | RS_OLDSTT+4 = RET (0C9h)                               |
| FB11h   | RS_OLDINT |  5     | Hook called by the RS-232C                             |
|         |           |        | RS_OLDINT = RST 30h (0F7h)                             |
|         |           |        | RS_OLDINT+1 = Slot ID                                  |
|         |           |        | RS_OLDINT+2 = Address                                  |
|         |           |        | RS_OLDINT+4 = RET (0C9h)                               |
| FB16h   | RS_DEVNUM |  1     | Byte offset. (RS-232C)                                 |
| FB17h   | RS_DATCNT |  1     | RS_DATCNT = (buffer count)                             |
| FB18h   | RS_BUFEND |  2     | RS_BUFEND = (buffer end ptr)                           |
| FB1Ah   | RS_ERRORS |  1     | RS-232C error code                                     |
| FB1Bh   | RS_FLAGS  |  1     | RS-232C RS_FLAGS                                       |
| FB1Ch   | RS_ESTBLS |  1     | Bit boolean. (RS-232C)                                 |
| FB1Dh   | RS_COMMSK |  1     | RS-232C mask                                           |
| FB1Eh   | RS_LSTCOM |  1     | Byte Data. (RS-232C)                                   |
| FB1Fh   | RS_LSTMOD |  1     | Byte Data. (RS-232C)                                   |

RS_FLAGS:
| BIT |   SET   |   RES   |
|-----|---------|---------|
|  0  | SI/SO ??          |
|  1  | RTS ON | RTS OFF  |
|  2  | XOFF    | XON     |
|  4  | ERROR?            |
|  7  | SHIFT OUT??       |

RS_ESTBLS
| BIT |   Description       |
|-----|---------------------|
|  0  | XON/XOFF            |
|  1  | SET -> USE RTS/CTS  |
|  3  | AUTO LF??           |
|  4  | SI/SO ??            |
|  7  | PRESENT             |

FOSSIL DATA VARIABLES:
| Address | Name   | Length | Description                                            |
|---------|--------|--------| -------------------------------------------------------|
| F3FCh   |        |  1     | 1st Marker Byte for fossile 'R'                        |
| F3FDh   |        |  1     | 2nd Marker Byte for fossile 'X'                        |
| F3FEh   |        |  2     | Fossil User Jump Table                                 |
| F400h   |        |  1     | Ext Fossil User Jump Table Slot                        |

