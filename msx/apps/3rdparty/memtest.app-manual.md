\newpage
\ifoot*{MEMTEST (c) 1995.5.16 Masatoshi Fujimoto}

### MEMTEST

```
(c) 1995.5.16 Masatoshi Fujimoto
```

MEMTEST allows to test your RAM on MSX-DOS. It is  Memory Mapper compatible.
It is an improved version of the program previously developed in BASIC.

#### Environment:
* MSX2, MSX2 + or MSX turbo R.
* MSX-DOS1. (Can not test all pages under MSX-DOS2 on Turbo R)

#### Specifications

1. For the test to be possible even where the program is loaded, the memory
     is tested without modifying the contents. The test reads a memory, inverts
     the bits and writes it then re-reads, re-inverts the bits, compares with
     the initial value read which is re-written if the test is valid, and so on
     for all the memory.

2. The checksum is calculated twice per page to check if data is corrupted or
  not. This is not able to detect all kinds of errors but it can find errors
  that can not be detected by the reading and writing test.

3. Since it is possible to specify the test slot, you can test the Memory
     Mapper of your choice.

4. Also check if the page (16KB) corresponds to the number entered in the
     Memory Mapper register.

#### How to use


`MEMTEST [#p[e]][/H]`

|Parameter|Description|
|---|---|
|#p|Specifies the primary slot to test (p is the primary slot number)|
|#pe|Specify the extended slot to test (e is the extended slot number)|
|/ H|Displays usable options.|


#### Examples

|Example|Description|
|---|-------|
|`MEMTEST #1` |Performs the test of the memory in the primary slot 1.|
|`MEMTEST #30`|Performs memory test in the 3-0 slot.|
|`MEMTEST`    |Performs the main memory test.|
|`MEMTEST /H` |Displays usable options.|

#### Displayed messages

MEMTEST displays messages during a test. This can help you if there is a
problem.

1. When the memory is OK

```
Memory Test Start.
Memory Slot Test #1               <- Test Slot Number
Memory Mapper Segment 00 - 3F     <- Pages available in hexadecimal
................................  <- Test progress bar
Memory Test Completed. Memory OK! <- Test finished without error found
```

2. When there has been a reading or writing error

```
Memory Test Start.
Memory Slot Test #1               <- Test Slot Number
Memory Mapper Segment 00 - 3F     <- Pages available in hexadecimal
.............                     <- Test progress bar
Memory Read / Write Error         <- Read or write error
> segment 04 offset 10FF          <- Page number and location of the error
........
Memory Test End.                  <- The test is finished

This means that part of the memory does not work correctly in reading or
writing. A RAM chip is probably defective.
```

3. When memory is corrupted

```
Memory Test Start.
Test Memory slot #1               <- Test Slot Number
Memory Mapper Segment 00 - 3F     <- Pages available in hexadecimal
..............                    <- Test progress bar
Memory Checksum Error             <- The checksum differs
> segment 04                      <- Page number where there was the error

Memory Test End.                  <- The test is finished
```

>   The checksum was calculated twice, they do not match.  The dynamic refresh circuit is probably defective.

4. When the Memory Mapper does not operate according to the standard

```
Memory Test Start.
Test Memory slot #1               <- Test Slot Number
Memory Mapper Segment 00 - 07     <- Pages available in hexadecimal
..............                    <- Test progress bar
```

> If you have a 1024KB Memory Mapper and 00 - 07 is displayed (only 128KB valid). Because there is only 3bit (out of the 8 bits of the Memory Mapper's register, please check the cabling in relation to the Mapper's registers.

5. When memory is not compatible Memory Mapper

```
Memory Test Start.
Test Memory slot #1               <- Test Slot Number
Not map RAM !!                    <- The present RAM is not a Memory
                                     Mapper
Memory Test End.                  <- The test is finished
```

6. When not in RAM

```
Memory Test Start.
Test Memory slot #1               <- Test Slot Number
Not RAM !!                        <- There is no RAM in this slot
Memory Test End.                  <- The test is finished
```

7. When the option /H is specified or when there is a parameter error

```
USAGE : MEMTEST [#pe | #p | /H]
  #pe : slot#(primary,expanded)
  #p  : slot#(primary)
  /H  : display usage
```

#### About the Memory Mapper

A Memory Mapper is an MSX2 standard RAM expansion. This memory is divided into
16K pages, 4 of which are allocated into 4 banks of the main memory. Each bank
is managed by an 8-bit register writable via the I/O ports: 0FCh, 0FDh, 0FEh
and 0FFh. This allows you to manage up to 4MB (16KB x 256) of memory in a slot.
If a Mapper is 512K, the calculation 512/16 will give the number of pages (32).
The value to write to one of the registers will be between 0 and 31. Never
read these registers to determine the size of a Memory Mapper! At best, you
will only get the size of the internal Memory Mapper.

I/O ports  Writing the page number of the Mapper to put on the memory Bank
```
  0FCH      Register for bank 0 (0000H ~ 3FFFH) (page 3 by default)
  0FDH      Register for bank 1 (4000H ~ 7FFFH) (page 2 by default)
  0FEH      Register for bank 2 (8000H ~ BFFFH) (page 1 by default)
  0FFH      Register for bank 3 (C000H ~ FFFEH) (page 0 by default)
```

#### Distribution

MEMTEST is a free software. Copyright has not been abandoned,
The use is free within the limits indicated below.

1. No guarantee
     The author declines any responsibility in case of damages due to the use
     of this software. However, please, you can send your remarks or problems
     encountered in as much detail as possible, this will improve the software.

2. Distribution rights
     You may distribute and use MEMTEST freely in your personal capacity but
     in no way sell it.

3. Other condition
     Do not modify the original content.

    1995 May 14  NIFTY-ID: MCN00585 Fujimoto Masatoshi


\newpage
\ifoot*{}

