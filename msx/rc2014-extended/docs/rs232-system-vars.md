https://www.msx.org/wiki/System_variables_and_work_area#RS-232C


RS-232C
Data Area Used By RS-232C

| Address | Name   | Length | Description                                            |
|---------|--------|--------| -------------------------------------------------------|
| FB03h   | RSTMP  |  1     | Temporary data storage for RS232 Driver                |
| FB03h   | TOCNT  |  1     | Counter used by the RS-232C interface                  |
| FB04h   | RSFCB  |  2     | FCB ("File Control Block") address of the RS-232C      |
| FB06h   | RSIQLN |  1     | Byte DATA used by the RS-232C interface                |
| FB07h   | MEXBIh |  5     | Hook called by the RS-232C                             |
|         |        |        | MEXBIh = RST 30h (0F7h)                                |
|         |        |        | MEXBIh+1 = Slot ID                                     |
|         |        |        | MEXBIh+2 = Address                                     |
|         |        |        | MEXBIh+4 = RET (0C9h)                                  |
| FB0Ch   | OLDSTT |  5     | Hook called by the RS-232C                             |
|         |        |        | OLDSTT = RST 30h (0F7h)                                |
|         |        |        | OLDSTT+1 = Slot ID                                     |
|         |        |        | OLDSTT+2 = Address                                     |
|         |        |        | OLDSTT+4 = RET (0C9h)                                  |
| FB0Ch   | OLDINT |  5     | Hook called by the RS-232C                             |
|         |        |        | OLDINT = RST 30h (0F7h)                                |
|         |        |        | OLDINT+1 = Slot ID                                     |
|         |        |        | OLDINT+2 = Address                                     |
|         |        |        | OLDINT+4 = RET (0C9h)                                  |
| FB16h   | DEVNUM |  1     | Byte offset. (RS-232C)                                 |
| FB17h   | DATCNT |  3     | DATA area. (RS-232C)                                   |
|         |        |        | DATCNT = Slot ID ???? (buffer count)                   |
|         |        |        | DATCNT+1 = Address                                     |
| FB1Ah   | ERRORS |  1     | RS-232C error code                                     |
| FB1Bh   | FLAGS  |  1     | RS-232C flags                                          |
| FB1Ch   | ESTBLS |  1     | Bit boolean. (RS-232C)                                 |
| FB1Dh   | COMMSK |  1     | RS-232C mask                                           |
| FB1Eh   | LSTCOM |  1     | Byte Data. (RS-232C)                                   |
| FB1Fh   | LSTMOD |  1     | Byte Data. (RS-232C)                                   |

FOSSIL DATA VARIABLES:
| Address | Name   | Length | Description                                            |
|---------|--------|--------| -------------------------------------------------------|
| F3FCh   |        |  1     | 1st Marker Byte for fossile 'R'                        |
| F3FDh   |        |  1     | 2nd Marker Byte for fossile 'X'                        |
| F3FEh   |        |  2     | Fossil User Jump Table                                 |
| F400h   |        |  1     | Ext Fossil User Jump Table Slot                        |

