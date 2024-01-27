This document extracted from https://www.msx.org/wiki/Extended_Bios (Based on work by Aoineko and Bengalack and others.)

# Extended Bios

1. Desciption of the EXTBIO entry
2. List of know ID with their name and functions
    1. 000h (0) - Broad-cast
    2. 004h (4) - Memory Mapper (MSX-DOS 2)
    3. 008h (8) - MSX-Modem & RS-232C
    4. 00Ah (10) - MSX-AUDIO
    5. 010h (16) - MSX-JE
    6. 011h (17) - Kanji driver
    7. 022h (34) - MSX-UNAPI (Unofficial)
    8. 04Dh (77) - MemMan (Unofficial)
    9. 084h (132) - μ-driver by Yoshikazu Yamamoto (Unofficial)
    10. 0D6H (214) - RC2014 API (Unofficial)
    11. 0F1h (241) - MultiMente (Unofficial)
    12. 0FFh (255) - System

## Description of the EXTBIO entry
EXTBIO entry has been added to the MSX standard to know how many and which extensions are connected to the system. It also makes it easier to manage them in machine language or in C.

The address of routine is 0FFCAh in the System Variable Slot. Most calls to EXTBIO will cause a call to the Bios of the corresponding extension.

Although we can think that the EXTBIO entry is specific to MSX-DOS 2, this is not the case. It being optional, it is necessary to check its presence before calling it by reading the variable HOKVLD (0FB20h). If bit 0 of this variable is zero, there is no compatible extension. If bit 0 of this variable is set but EXTBIO (0FFCAh) contains 0C9h, there is no routine installed.

```
0FFCAh  EXTBIO
    Entry:  D = Device identifier.
            E = Number of the function to call.
    Note:   In general, function 0 aims to create a table to know how many and
            which extensions are connected to the system.

0FFCFh  DISINT
    Entry:  Nothing
    Note:   Called by the function 2 of Broad-cast.

0FFD4h  ENAINT
    Entry:  Nothing
    Note:   Called by the function 2 of Broad-cast.

0FFD9h  DISINT/ENAINT routines (14 bytes of length)
```

It is possible to add or an extended BIOS routine for any device by doing the following.

1. Read bit 0 of the byte at address 0FB20h (HOKVLD). If it is 0, set it to 1 then write 0C9h (RET instruction) on the 29 bytes from 0FFCAh and go to step 3.
2. Copy the 5 bytes from 0FFCAh to an area of your choice.
3. Write the jump instructions to your extended BIOS routine at 0FFCAh. Your routine should end with a jump to the previously copied area (in step 2) or a RET if step 2 has been skipped.
4. Create the routines DISINT and ENAINT if they are not present.

> For more information, please refer to the description of the extended BIOS of each device.

# Known Extended Bios APIs

## 000h (0) - Broad-cast

### Function 0:

<dl>
<dt>Use</dt>
<dd>Create a table containing device numbers with extended BIOS.</dd>
<dt>Entry</dt>
<dd>B = Slot in which to create the table</br>
        HL = Address of the table to create
</dd>
<dt>Output</dt>
<dd>B = Slot wherein is the first byte following the table</br>
    HL = Address of next byte after the table
</dd>
<dt>Modifies:</dt><dd>All registers</dd>
<dt>Note:</dt>
<dd>The resulting table contains the available device numbers each spaced apart by a reserved byte (00h). Broad-cast (0) and System (255) do not appear in the table.
Here is an example to get the device table:
</dl>

```
RAMAD0  equ     0F341h          ; Slot of the Main-Ram on page 0~3FFFh
extbio  equ     0ffcah          ; Call address to an extended Bios

getdev:
        ld      hl, table
        call    getslt          ; B = number the slot of the table
        ld      d, 0            ; Broad-cast device
        ld      e, 0            ; Function 'get device number'
        jp      extbio

; Routine getslt
; Entry: HL = Address in Ram
; Output: B = Slot number corresponding to the HL address
; Modifies: A and B

getslt:
        push    hl
        ld      a, h
        rla
        rla
        rla                     ; Bit 6-7 to bit 1-0
        and     3               ; Reset unused bits
        ld      c, a
        ld      b, 0
        ld      hl, RAMAD0
        add     hl, bc
        ld      b, (hl)         ; B = Slot Number of Main RAM
        pop     hl
        ret

table:
        ds      32              ; Reserve 32 bytes for the table
```

### Function 1:
<dl>
<dt>Use:</dt>
<dd>    Get the number of trap to place the ON ... GOSUB Basic instructions to insert a personal routine in machine language that will be performed during a jump performed by an ON ... GOSUB.
</dd>
<dt>Entry:</dt> <dd> A = 0</dd>
<dtOutput:<dt>
<dd>A = trap number

    0~9 for ON KEY GOSUB
    10 for ON STOP GOSUB
    11 for ON SPRITE GOSUB
    12~16 for ON STRIG GOSUB
    17 for ON INTERVAL GOSUB
    18~23 for extended devices
    24~25 Reserved for the system
</dd>
<dt>Modifies:</dt><dd>       F and DE</dd>
</dl>

#### Function 2:
<dl>
<dt>Use:</dt><dd>    Disable the interrupts from all devices.</dd>
<dt>Entry:</dt><dd>  Nothing</dd>
<dt>Output:</dt><dd> Nothing</dd>
<dt>Modifies:</dt><dd>       ?</dd>
<dt>Note:</dt><dd>   This routine helps prevent transfer errors from devices that are sending an interrupt signal. This happens, for example, with the RS-232C when interrupting the CPU interrupts too long.
</dd>
</dl>
> Example of the application program calls a "declaration of interrupt disabled" extended BIOS:

```
hokvld  equ 0fb20h      ;address of Extended BIOS valid flag
extbio  equ 0ffcah      ;entry address of extended BIOS

        ld a,(hokvld)   ;get valid flag
        rrca            ;is the extended BIOS valid?
        jr nc,noextbio  ;no..
        ld d,0          ;broad­cast command
        ld e,2          ;say 'disable interrupt'
        call extbio     ;call extended BIOS
           .
           .
;
;Enters here when no extended BIOS
;
noextbio:
        di
        halt            ;halt here
```

### Function 3:
<dl>
<dt>Use:</dt><dd>    Enable the interrupts from all devices.</dd>
<dt>Entry:</dt><dd>  Nothing</dd>
<dt>Output:</dt><dd> Nothing</dd>
<dt>Modifies:</dt><dd>       ?</dd>
</dl>

## 004h (4) - Memory Mapper (MSX-DOS 2)
To use these routines under MSX-DOS1 you need separate μ-driver (MU.COM) from Yoshikazu Yamamoto.

### Function 0:
<dl>
<dt>Use:</dt><dd>    Get the information table on the main Memory Mapper.</dd>
<dt>Entry:</dt><dd>  B = Slot in which to create the table.</br>
HL = Address of the table to create.
</dd>
<dt>Output:</dt><dd>B = Slot in which is the next byte after the table.</br>
HL = Address of the next byte after the table.
</dd>

<dt>Modifies:</dt><dd>       All registers</dd>
<dt>Note:</dt><dd>   The information table on the main Memory Mapper has the following format.

    HL+0 = Main Memory Mapper slot Number (Address given as entry)
    HL+1 = Least significant byte of address of the jump table
    HL+2 = Most significant byte of address of the jump table
    HL+3 = Number of free segments
    HL+4 = Number of segments
    HL+5 = Reserved
    HL+6 = Reserved
    HL+7 = Reserved
    HL+8 = ... (Address in HL as return)
</dd>
</dl>

### Function 1:
<dl>
<dt>Use:</dt><dd>    Get the variable table from all Memory Mappers.</dd>
<dt>Entry:</dt><dd>  A=0</dd>
<dt>Output:</dt><dd> A = Slot number of the main Memory Mapper, 0 if mapper support routines are not available.<br>
HL = Address of the variable table
</dd>
<dt>Modifies:</dt><dd>       All registers</dd>
<dt>Note:</dt><dd>   Example of a variable table with two connected Memory Mappers.

    HL+0 = Main Memory Mapper slot Number (Address in HL as output)
    HL+1 = Number of segments
    HL+2 = Number of free segments
    HL+3 = Number of segments reserved by the system
    HL+4 = Number of segments reserved by the user
    HL+5 = Reserved
    HL+6 = Reserved
    HL+7 = Reserved
    HL+8 = Second Memory Mapper slot Number
    HL+9 = Number of segments
    HL+10 = Number of free segments
    HL+11 = Number of segments reserved by the system
    HL+12 = Number of segments reserved by the user
    HL+13 = Reserved
    HL+14 = Reserved
    HL+15 = Reserved
    HL+16 = 0 (end of Memory Mapper slots enumeration)
On MSX turbo R, the main Memory Mapper will always be the internal Memory Mapper.
</dd>
</dl>


### Function 2:
<dl>
<dt>Use:</dt><dd>    Get the jump table to the routines of the Memory Mapper.</dd>
<dt>Entry:</dt><dd>  A = 0</dd>
<dt>Output:</dt><dd> A = Number of segments of the main Memory Mapper</br>
B = Slot number of the main Memory Mapper</br>
C = Number of free segments of the main Memory Mapper</br>
HL = Jump table address
</dd>
<dt>Modifies:</dt><dd>       All registers exept B</dd>
<dt>Note:</dt><dd>   Format of the jump table is as follows.

```
HL+0h   ALL_SEG
         Use: Allocate a segment
         Entry: A=0 for the user, A=1 for the system
               B = Mapper slot in the form FxxxSSPP
                   If xxx=000, allocate a segment to the specified slot
                   If xxx=001, allocate a segment in another slot than the one specified
                   If xxx=010, allocate a segment to the specified slot if free,
                    otherwise in another slot
                   If xxx=011, allocate a segment in another slot than the one
                    specified if free otherwise try in specified slot
               B = 0 for main Memory Mapper
         Output:  F = Carry is set if nothing has been allocated
                 A  = Segment number
                 B  = Slot number

HL+3h   FRE_SEG
         Use: Free a segment
         Entry: A = Segment number
                B = Mapper slot in the form FxxxSSPP (0 for the main Memory Mapper)
         Output: F = Carry is set if nothing has been freed

HL+6h   RD_SEG
         Use: Read a byte in a segment
         Entry: A = Segment number
                HL = Address to read from the segment
         Output: A = Byte read
         Modifies: AF
         Note: This routine disables interrupts

HL+9h   WR_SEG
         Use: Write a byte in a segment
         Entry: A = Segment number
                HL = Address to write on the segment
         Modifies: AF
         Note: This routine disables interrupts

HL+Ch   CAL_SEG
         Use: Inter segment call
         Entry: IYh = Segment number
                IX = Address
         Modifies: All except AF, BC, DE and HL
         Note: AF, BC, DE and HL can be used as parameters for the routine to call.

HL+Fh   CALLS
         Use: Inter segment call
         Entry: The three bytes following the instruction call CALLS
                 eg:    call CALLS
                         db Slot ID
                         dw ADDRESS
         Modifies: All except AF, BC, DE and HL
         Note: AF, BC, DE and HL can be used as parameters for the routine to call.

HL+12h  PUT_PH
         Use: Select a segment on the corresponding memory page at the specified address.
         Entry: HL = Address (in fact the bits 6~7 of H specify the page number)
                A = Segment number
         Modifies: Nothing

HL+15h  GET_PH
         Use: Get the selected segment number on the corresponding memory page at the
                 specified address.
         Entry: HL = Address (in fact the bits 6~7 of H specify the page number)
         Output: A = Segment number
         Modifies: Nothing

HL+18h  PUT_P0
         Use: Select a segment on page 0 (0000h~3FFFh)
         Entry: A = Segment number
         Modifies: Nothing

HL+1Bh  GET_P0
         Use: Get the segment number on page 0 (0000h~3FFFh)
         Output: A = Segment number
         Modifies: A

HL+1Eh  PUT_P1
         Use: Select a segment on page 1 (4000h~7FFFh)
         Entry: A = Segment number
         Modifies: Nothing

HL+21h  GET_P1
         Use: Get the segment number on page 1 (4000h~7FFFh)
         Output: A = Segment number
         Modifies: A

HL+24h  PUT_P2
         Use: Select a segment on page 2 (8000h~BFFFh)
         Entry: A = Segment number
         Modifies: Nothing

HL+27h  GET_P2
         Use: Get the segment number on page 2 (8000h~BFFFh)
         Output: A = Segment number
         Modifies: A

HL+2Ah  PUT_P3
         Use: Select a segment on page 3 (C000h~FFFFh)
         Entry: A = Segment number
         Modifies: Nothing

HL+2Dh  GET_P3
         Use: Get the segment number on page 3 (C000h~FFFFh)
         Output: A = Segment number
         Modifies: A

```
</dd>
</dl>


## 008h (8) - MSX-Modem & RS-232C

### Function 0:
<dl>
<dt>Use:</dt>  <dd>  Get a table of information about the MSX-Modem and RS-232C installed.</dd>
<dt>Entry:</dt>  <dd>B = Slot in which to create the table<br>
HL = Address of the table to create
</dd>
<dt>Output:</dt><dd> B = Slot in which is the next byte below the table<br>
HL = Address of the next byte below the table
</dd>
<dt>Modifies:</dt>    <dd>   All registers</dd>
<dt>Note:</dt> <dd>  Here is an example of a table with two interfaces installed.<br>

```
HL+0 = Slot number of first interface found (Address indicated by HL as input)
HL+1 = Least significant byte of address of the jump table
HL+2 = Most significant byte of address of the jump table
HL+3 = Reserved (00h)

HL+4 = Slot number of second interface
HL+5 = Least significant byte of address of the jump table
HL+6 = Most significant byte of address of the jump table
HL+7 = Reserved (00h)

HL+8 = ... (Address in HL as return)
```
</dd>
</dl>

## 00Ah (10) - MSX-AUDIO

### Function 0:
<dl>
<dt>Use:  </dt><dd>  Get the information table about installed MSX-AUDIO.</dd>
<dt>Entry:</dt><dd>  B = Slot in which to create the table<br>
HL = Address of the table to create</dd>

<dt>Output: <dd>B = Slot in which is the next byte below the table<br>
HL = Address of the next byte below the table
</dd>
<dt>Modifies:</dt><dd>       F</dd>
<dt>Note:</dt><dd>   Below is an example of a table with two MSX-AUDIO installed.<br>

```
HL+0 = Slot Number of first MSX-AUDIO (Address indicated by HL as input)
HL+1 = Least significant byte of pointer to the jump table
HL+2 = Most significant byte of pointer to the jump table

HL+3 = Slot Number of second MSX-AUDIO
HL+4 = Least significant byte of pointer to the jump table
HL+5 = Most significant byte of pointer to the jump table

HL+6 = ... (Address in HL as output)
```
</dd></dl>

### Function 1:
<dt>Use:   </dt><dd> Get the table of jumps to the routines of the MSX-AUDIO.</dd>
<dt>Entry: </dt><dd> A=0</br>
B = Slot in which to create the table</br>
HL = Address of the table to create</dd>
<dt>Output:</dt><dd> A = Number of MSX-AUDIO installed<br>
HL = Address of jumps table</dd>

<dt>Modifies:</dt><dd>       All register except B</dd>
<dt>Note:</dt><dd>   Created table of jumps is as below.<br>

```
HL+0h   VERSION
         Use: These 3 bytes indicate the ROM version.

HL+3h   MBIOS
         Use: Call to a MBIOS routine
         Entry: HL = Address of the MBios routine to call
                For routines that use IX and IY as a parameter, please set these
                parameters in BUF (0F55Eh) as follows.
                BUF = 8 least significant bits of IX
                BUF+1 = 8 most significant bits of IX
                BUF+2 = 8 least significant bits of IY
                BUF+3 = 8 most significant bits of IY
         Output: See the MBIOS routines

HL+6h   AUDIO
         Use: Initialize the MSX-AUDIO. The routines of the MSX-AUDIO will only be usable
              after initialization.
         Entry: Define the following parameters in BUF (0F55Eh).
                BUF = Number of MML channels per FM voice for PLAY. (0~9)
                BUF+1 = Switch mode
                        bit 0 to enable the drum machine
                        bit 1 to enable PLAY's ADPCM
                        bit 2 to enable the CMS mode of the MSX-AUDIO
                BUF+2 = Number of FM voices for instruments. (0~9)
                BUF+3 = Number of FM voices in the first MML channel. (0~9)
                BUF+4 = Number of FM voices in the second MML channel. (0~8)
                BUF+5 = Number of FM voices in the third MML string. (0~7)
                BUF+6 = Number of FM voices in the fourth MML channel. (0~6)
                BUF+7 = Number of FM voices in the fifth MML channel. (0~5)
                BUF+8 = Number of FM voices in the sixth MML channel. (0~4)
                BUF+9 = Number of FM voices in the seventh MML channel. (0~3)
                BUF+10 = Number of FM voices in the eighth MML channel. (0~2)
                BUF+11 = Number of FM voices in the ninth MML channel. (0~1)
         Output: Carry goes to 1 in case of parameter error
         Note: When the drum machine is enabled, the number of FM voices increases
               from 9 to 6.

HL+9h   SYNTHE
         Use: Transfer control to the supplied application program. This
              routine will have no effect when the AUDIO routine is called.
         Entry: Nothing
         Output: Nothing

HL+0Ch  PLAYF
         Use: Examine the state of MML playing.
         Entry: A = Voice number. (0 for all voices)
         Output: HL = FFFFh if reading in progress otherwise HL = 0000h.

HL+0Fh  BGM
         Use: Enable/Disable BGM mode (when background play).
         Entry: A = Enable/Disable BGM mode. (1/0)
         Output: Nothing
         Note: Below are the functions that can work in BGM mode.
                 - Play an MML channel.
                 - Reading an ADPCM record from the local mode.
                 - Play a recording of the musical keyboard at the specified address.

HL+12h  MKTEMPO
         Use: Setting the tempo to play or record a musical keyboard tune.
         Entry: DE = Number of quarter notes per minute. (25~360)
         Output: F = Carry is set if there is a parameter error.

HL+15h  PLAYMK
         Use: Play an air with the musical keyboard.
         Entry: BC = Start address of the data recorded with the music keyboard,
                DE = End address of the data to recorded with the musical keyboard.
         Output: Nothing

HL+18h  RECMK
         Use: Record an air with the musical keyboard.
         Entry: BC = Start address of the data to be recorded on the music keyboard.
                DE = End address of the data to be recorded on the musical keyboard.
         Output: Nothing

HL+1Bh  STOPM
         Use: Stop playing a music keyboard recording, an ADPCM/PCM recording, or
              an MML channel played.
         Entry: Nothing

         Output: Nothing
         Note: It is possible to resume playing a recording on the keyboard musical by
               calling CONTMK.

HL+1Eh  CONTMK
         Use: Continue playing a musical keyboard recording, ADPCM/PCM, or an MML channel
              played.
         Entry: Nothing

         Output: Nothing

HL+21h  RECMOD
         Use: Set the music keyboard mode.
         Entry: A = Mode
                         0 for mute (does not record). 1 for recording,
                         2 to play the air,
                         3 to play while recording.
         Output: F = Carry is set if parameter error.

HL+24h  STPPLY
         Use: Stop playing an MML channel.
         Entry: Nothing

         Output: Nothing

HL+27h  SETPCM
         Use: Initialize the ADPCM / PCM audio file.
         Entry: Define the following parameters into BUF (0F55Eh).
                 BUF = Number of record. (0~15)
                 BUF+1 = Number of device to use. (0~3 or 5)
                 BUF+2 = Mode. (0 or 1)
                 BUF+3 = If device number is 1 or 3 then put here a number of sound in ROM.
                         If device number is 5, indicate here the 8 least significant
                         bits of the VRAM address.
                 BUF+4 = If device number is 1 or 3 then put 0. If it's 5, indicate the
                         8 most significant bits of the VRAM address.
                 BUF+5 = 8 least significant bits of record length.
                 BUF+6 = 8 most significant bits of the length of the record.
                 BUF+7 = 8 least significant bits of the sampling frequency.
                 BUF+8 = 8 most significant bits of the sampling frequency.
                 BUF+9 = Channel. (0 or 1)
         Output: Carry is set if parameter error.
         Note: Device 4 (the CPU) is not usable.

HL+2Ah  RECPCM
         Use: Record a sound into a file.
         Entry: Define the following parameters in BUF (0F55Eh).
                 BUF = Number of record. (0~15)
                 BUF+1 = Synchro. (0 or 1)
                 BUF+2 = 8 least significant bits of offset.
                 BUF+3 = 8 most significant bits of offset.
                 BUF+4 = 8 least significant bits of record length.
                 BUF+5 = 8 most significant bits of the length of record.
                 BUF+6 = 8 least significant bits of the sampling frequency.
                 BUF+7 = 8 most significant bits of the sampling frequency.
                 BUF+8 = Channel. (0 or 1)
         Output: Carry is set if parameter error.
         Note: Sampling frequency can be set with SETPCM. If so, set 0FFh to BUF+6 and
               BUF+7.

HL+2Dh  PLAYPCM
         Use: Play a recording.
         Entry: Define the following parameters in BUF (0F55Eh).
                 BUF = Number of record. (0~15)
                 BUF+1 = Loop playback. (0 or 1)
                 BUF+2 = 8 least significant bits of offset.
                 BUF+3 = 8 most significant bits of offset.
                 BUF+4 = 8 least significant bits of record length.
                 BUF+5 = 8 most significant bits of the length of record.
                 BUF+6 = 8 least significant bits of the sampling frequency.
                 BUF+7 = 8 most significant bits of the sampling frequency.
                 BUF+8 = Channel. (0 or 1)
         Output: Carry is set if parameter error.
         Note: Sampling frequency can be set with SETPCM. If so, set 0FFh to BUF+6 and
               BUF+7.

HL+30h  PCMFREQ
         Use: Change the playback frequency.
         Entry: BC = Frequency of the first channel in Hz. (1800~49716)
                DE = Frequency of the second channel in Hz. (1800~49716)
         Output: Carry is set if parameter error.

HL+33h  MKPCM
         Use: Set the ADPCM file number played by the music keyboard.
         Entry: A = Record number. (0~15 or 0FFh for none)
         Output: Carry is set if parameter error.
         Note: Only files in local mode are playable.

HL+36h  PCMVOL
         Use: ADPCM / PCM sound volume adjustment to play.
         Entry: BC = Volume of the first channel. (0~63)
                DE = Volume of the second channel. (0~63)
         Output: Carry is set if parameter error.
         Note: When there is no second channel, the DE must contain the same value as BC.
               By default, the ADPCM volume is 63, the PCM volume is 32.

HL+39h  SAVEPCM
         Use: Save an ADPCM / PCM record to disk.
         Entry: A = Record number
                HL = Address pointing to the filename in quotation marks.
         Output: F = Carry is set when record number is bad.
         Notes:
         - In assembler, we define the file name as follows.
            FILENAME: DB       22H,"A:TRACK.PCM",22H,0
         - In the event of a disk error (wrong file name, disk not inserted, etc.), the
           management will be transferred to the error handling routine of the BASIC
           interpreter. You can intercept it with Hook H.ERRO (0FFB1h).
         - The created file will be constituted as follows.
            First, there is 7 bytes as the header for binary files saved up by the
            instruction BSAVE of the Basic followed by 8 bytes including information about
            the stored ADPCM/PCM data. The rest will be the data of the digitized sound.

HL+3Ch  LOADPCM
         Use: Loading an ADPCM/PCM record from disk made with SAVEPCM.
         Entry: A = Record number.
                HL = Address pointing to the filename in quotation marks.
         Output: Nothing
         Notes:
         - In assembler, we define the file name as follows.
            FILENAME: DB       22H,"A:TRACK.PCM",22H,0
         - In the event of a disk error (wrong file name, disk not inserted, etc.), the
           management will be transferred to the error handling routine of the BASIC
           interpreter. You can intercept it with Hook H.ERRO (0FFB1h).

HL+3Fh  COPYPCM
         Use: Copies data from an ADPCM/PCM record.
         Entry: Define the following parameters in BUF (0F55Eh).
                 BUF = Number of source record. (0~15)
                 BUF+1 = Number of destination record. (0~15)
                 BUF+2 = 8 least significant bits of source record.
                 BUF+3 = 8 most significant bits of source record.
                 BUF+4 = 8 least significant bits of record length.
                 BUF+5 = 8 most significant bits of the length of record.
                 BUF+6 = 8 least significant bits of destination record.
                 BUF+7 = 8 most significant bits of destination record.
                 BUF+8 = Channel of source. (0 or 1)
         Output: Carry is set if parameter error.

HL+42h  CONVP
         Use: Convert a PCM record to ADPCM format.
         Entry: Define the following parameters in BUF (0F55Eh).
                BUF = Number of the source record. (0~15)
                BUF+1 = Number of the destination record. (0~15)
         Output: Carry is set if parameter error.

HL+45h  CONVA
         Use: Convert an ADPCM record to PCM format.
         Entry: Define the following parameters in BUF (0F55Eh).
                BUF = Number of the source record. (0~15)
                BUF+1 = Number of the destination record. (0~15)
         Output: Carry is set if parameter error.

HL+48h  VOICE
         Use: Set the tone for each FM sound voice.
         Entry: Define the following parameters in BUF (0F55Eh).
                BUF = Parameter block of Voice 1.
                BUF+4 = Parameter block of Voice 2.
                BUF+8 = Parameter block of Voice 3.
                BUF+12 = Parameter block of Voice 4.
                BUF+16 = Parameter block of Voice 5.
                BUF+20 = Parameter block of Voice 6.
                BUF+24 = Parameter block of Voice 7.
                BUF+28 = Parameter block of Voice 8.
                BUF+32 = Parameter block of Voice 9.
                BUF+36 = End (0FFh)
                Each block consists of 4 bytes as follows.
                 +0 = FM voice number. (0~8)
                 +1 = set to 00h.
                 +2 = Sound number of library. (0~63)
                 +3 = Set to 00h.
                Blocks corresponding to a user-defined sound as follows.
                 +0 = FM voice number. (0~8)
                 +1 = Set to FFh.
                 +2 = 8 least significant bits of a record address.
                 +3 = 8 most significant bits of a record address.
         Output: Nothing

HL+4Bh  VOICECOPY
         Use: Copy data from an FM voice.
         Entry: 1. Copy a sound from the library to another sound of the library.
                   BUF+0 = 0
                   BUF+1 = Sound number of source library. (0~63)
                   BUF+2 = 0
                   BUF+3 = 0
                   BUF+4 = 0
                   BUF+5 = 0
                   BUF+6 = Sound number of destination library. (32~63)
                   BUF+7 = 0
                   BUF+8 = 0
                   BUF+9 = 0
                2. Copy a sound from the library to a user's sound.
                    BUF+0 = 0
                   BUF+1 = Sound number of source library. (0~63)
                   BUF+2 = 0
                   BUF+3 = 0
                   BUF+4 = 0
                   BUF+5 = 0FFh
                   BUF+6 = 8 least significant bits of destination address.
                   BUF+7 = 8 most significant bits of destination address.
                   BUF+8 = 0
                   BUF+9 = 0
                3. Copy a recording to a sound from the library.
                   BUF+0 = 0FFh
                   BUF+1 = 8 most significant bits of source record address.
                   BUF+2 = 8 most significant bits of source record address.
                   BUF+3 = 0
                   BUF+4 = 0
                   BUF+5 = 0
                   BUF+6 = Sound number of destination library. (32~63)
                   BUF+7 = 0
                   BUF+8 = 0
                   BUF+9 = 0
                4. Copy library sounds from 32 to 63 to a recording.
                    BUF = 0
                   BUF+1 = 0FFh
                   BUF+2 = 0
                   BUF+3 = 0
                   BUF+4 = 0
                   BUF+5 = 0FFh
                   BUF+6 = 8 least significant bits of destination address.
                   BUF+7 = 8 most significant bits of destination address.
                   BUF+8 = 8 least significant bits of record length.
                   BUF+9 = 8 most significant bits of record length.
                5. Copy a recording to library sounds 32 to 63.
                   BUF = 0FFh
                   BUF+1 = 8 least significant bits of source record address.
                   BUF+2 = 8 most significant bits of source record address.
                   BUF+3 = 8 least significant bits of record length.
                   BUF+4 = 8 most significant bits of record length.
                   BUF+5 = 0
                   BUF+6 = 0FFh
                   BUF+7 = 0
                   BUF+8 = 0
                   BUF+9 = 0
         Output: Nothing
```
</dd>
</dl>

## 010h (16) - MSX-JE

### Function 0:
<dl>
<dt>Use:</dt><dd>    Set input work area for Japanese characters. (Available on some Japanese MSX computers, Kanji cartridges and Japanese word processors with MSX-JE.)</dd>
<dt>Entry: </dt><dd> B = Slot number of the work area of the characters entered by RETURN.<br>
HL = Pointer on the 64-byte work area.</dd>
<dt>Output: </dt><dd> = Slot number of next work area.<br>
HL = Pointer on the next work area.</dd>
<dt>Modifies:</dt><dd>       F</dd>
<dt>Note:</dt><dd>   Stack must be on the page 3 (C000h~FFFFh). Therefore, the access point searches for the input address as follows.<dd>
</dl>

Sample program to get MSX-JE extended BIOS input address:

```
HOKVLD  equ     0fb20h  ;Extended BIOS flag
EXTBIO  equ     0ffcah  ;Extended BIOS hook
;
        ld      a,(HOKVLD)
        rra
        jr      nc,NO_MJE
        ld      hl,MJETBL
        call    GETSLT  ;Get slot address of MJETBL into [B]
        ld      de,16*256+0
        push    hl
        call    EXTBIO
        pop     de
        or      a
        sbc     hl,de
        jr      z,NO_MJE
        .
        .
        .
```

When returning from EXTBIO, the data is returned to MJETBL as follows, HL points the byte behind the table. When the MSX-JE function does not exist, HL does not change. The following table is an example with two MSX-JEs.

```
HL+0 = Capacity vector (Address indicated by HL as input)
HL+1 = Slot Number of first MSX-JE
HL+2 = Least significant byte of pointer to the jump table
HL+3 = Most significant byte of pointer to the jump table

HL+4 = Capacity vector
HL+5 = Slot Number of second MSX-JE
HL+6 = Least significant byte of pointer to the jump table
HL+7 = Most significant byte of pointer to the jump table

HL+8 = ... (Address in HL as return)
```

## 011h (17) - Kanji driver
### Function 0:
<dl>
<dt>Use:</dt><dd>    ?</dd>
<dt>Entry:</dt><dd>  ?</dd>
<dt>Output:</dt><dd> ?</dd>
<dt>Modifies:</dt><dd>       ?</dd>
</dl>

## 022h (34) - MSX-UNAPI (Unofficial)
### Function 0:
<dt>Use:</dt><dd>    See MSX-UNAPI site</dd>
<dt>Entry:</dt><dd>  ?</dd>
<dt>Output:</dt><dd> ?</dd>
<dt>Modifies:</dt><dd>       ?</dd>

## 04Dh (77) - MemMan (Unofficial)
### Function 0:
<dl>
<dt>Use: </dt><dd>   See MemMan documentation.</dd>
<dt>Entry: </dt><dd> ?</dd>
<dt>Output: </dt><dd>?</dd>
<dt>Modifies: </dt><dd>      ?</dd>
</dl>

## 084h (132) - μ-driver by Yoshikazu Yamamoto (Unofficial)

Call GET_VER first to confirm if it is installed before using other extended BIOS functions related.

### Function 0:
<dl>
<dt>Use:</dt><dd>    Get version number of μ-driver</dd>
<dt>Entry:</dt><dd>  A = 0</br>
D = 84H</dd>
<dt>Output: </dt><dd>A = 1 when installed, 0 if not<br>
HL = Version number (H = Major version / L = Minor version)</dd>
<dt>Modifies:</dt><dd>       ?</dd>
</dl>

### Function 10H:
<dl>
<dt>Use:  </dt><dd>  Allocate a segment for the system</dd>
<dt>Entry: </dt><dd> D = 84H</dd>
<dt>Output: </dt><dd>Cy = 0:Successful / 1:Failed</br>
A = Segment number<br>
B = Slot number</dd>
<dt>Modifies: </dt><dd>      ?</dd>
</dl>

### Function 11H:
<dl>
<dt> Use:  </dt><dd>  Free a segment</dd>
<dt> Entry:</dt><dd>  A = Segment number</br>
B = Slot number<br>
D = 84H</dd>
<dt>Output:</dt><dd> Cy = 0:Successful / 1:Failed</dd>
<dt>Modifies:</dt><dd>       ?</dd>
<dt>Note:</dt><dd>   Use this function to free a segment for the system or user</dd>
</dl>

### Function 20H:
<dl>
<dt>Use:</dt><dd>    Allocate a segment for the user</dd>
<dt>Entry:</dt></dd>  D = 84H</dd>
<dt>Output:</dt><dd> Cy = 0:Successful / 1:Failed</br>
A = Segment number<br>
B = Slot number<dd>
<dt>Modifies:</dt><dd>       ?</dd>
</dl>

### Function 21H:
<dl>
<dt>Use:</dt><dd>    Release all user's segments</dd>
<dt>Entry:</dt><dd>  D = 84H</dd>
<dt>Output:</dt><dd> None</dd>
<dt>Modifies:</dt><dd>       ?</dd>
</dl>

## 0D6 (214) - RC2014 API
A set of hardware specific interface apis for the Yellow MSX homebrew computer

### Function 1: (RC2014_INSTALL_FOSSIL)
<dl>
<dt>Use:</dt><dd>Install a fossil driver for SIO/2 RC2014 module</dd>
</dl>

### Function 2: (RC2014_SIO_GET_CLOCK)
<dl>
<dt>Use:</dt><dd>Retrieve the current clock multiplier configured value within the SIO/2 chip</dd>
</dl>

### Function 3: (RC2014_SIO_SET_CLOCK)
<dl>
<dt>Use:</dt><dd>Configure the current clock multiplier within the SIO/2 chip</dd>
</dl>

### Function 4: (RC2014_SIO_GET_PRESENT)
<dl>
<dt>Use:</dt><dd>Return the result of the hardware detection of the SIO/2 RC2014 module</dd>
<dt>Output:</dt><dd>L = 1 if present, 0 if not</br>
</dl>

### Function 0x81 (129): (RC2014_USB_GET_PRESENT)
Return the result of the hardware detection of the USB Module

<dl>
<dt>Use:</dt><dd>Retrieve hardware probe status of the USB Module</dd>
<dt>Entry:</dt><dd>A - the address the device has been configured to respond to</br>
HL -a buffer to store the device descriptor in - must be in the top page</dd>
<dt>Output:</dt><dd>L == 0 if no error, otherwise the error code</dd>
</dl>

### Function 0x82 (130): (RC2014_USB_CONTROL_TRANSFER)

<dl>
<dt>Use:</dt><dd>Perform a USB control transfer (in or out)</br>
  See https://www.beyondlogic.org/usbnutshell/usb4.shtml for a description of the USB control transfer</dd>
<dt>Entry:</dt><dd></br>
HL = address within page 3, that references the parameter for the function<br>

```
HL+0 cmd_packet         = address of command packet to be sent
HL+2 buffer             = ddress of data to be sent or received
HL+4 device_address     = address of the target USB device
HL+5 max_packet_size    = packet size to be used for the control transfer
```

all pointers and buffers must be in page 3<br>
</dd>
<dt>Output:</dt><dd>L = 0 if success</br>
</dl>

> The current solution does not support hot plugging/removal of USB devices.  So any change to USB's current assigned address or configuration will result incorrect behaviour of the system.

### Function 0x83 (131): (RC2014_USB_OUT_TRANSFER)

<dl>
<dt>Use</dt><dd>Perform a USB data out transfer</dd>
</dl>

### Function 0x84 (132): (RC2014_USB_IN_TRANSFER)

<dl>
<dt>Use</dt><dd>Perform a USB data in transfer</dd>
</dl>

## 0F1h (241) - MultiMente (Unofficial)
### Function 0:
<dl>
<dt>Use:</dt><dd>    ? (used by MultiMente)</dd>
<dt>Entry:</dt><dd>  ?</dd>
<dt>Output:</dt><dd> ?</dd>
<dt>Modifies:</dt><dd>       ?</dd>
</dl>


## 0FFh (255) - System
### Function 0:
<dl>
<dt>Use:</dt><dd>    Get a table of information about extended Bios added to the system:</dd>
<dt>Entry:</dt><dd>  A = 0<br>
B = Slot in which to create the table<br>
HL = Address of the table to create</dd>
<dt>Output: </dt><dd>B = Slot in which is the first byte following the table<br>
HL = Address of next byte after the table</dd>
<dt>Modifies:  </dt><dd>     All registers.<dd>
<dt>Note: </dt><dd>  Format of the extended BIOS information table of system.<br>
</dl>

```
HL+0 = Device Slot Number (Address indicated by HL as input)
HL+1 = Least significant byte of address of the jump table
HL+2 = Most significant byte of address of the jump table
HL+3 = Manufacturer's ID (See list below)
HL+4 = Reserved

HL+5 = Device Slot Number
HL+6 = Least significant byte of address of the jump table
HL+7 = Most significant byte of address of the jump table
HL+8 = Manufacturer's ID for next same device
HL+9 = Reserved

HL+10 = ... (Address in HL as return)
```

List of official manufacturer IDs:

```
0 = ASCII               9 = Mitsubishi          18 = Spectravideo
1 = Microsoft           10 = Nippon Denki       19 = Toshiba
2 = Canon               11 = Yamaha             20 = Mitsumi
3 = Casio               12 = Victor             21 = Telematica
4 = Fujitsu             13 = Philips            22 = Gradiente
5 = General             14 = Pioneer            23 = Sharp Epcom
6 = Hitachi             15 = Sanyo              24 = Goldstar
7 = Kyocera             16 = Sharp              25 = Daewoo
8 = National/Panasonic  17 = Sony               26 = Samsung
```

### Function 1:
<dl>
<dt>Use:</dt><dd>    Number of installed drives.</dd>
<dt>Entry:</dt><dd>  Nothing</dd>
<dt>Output:</dt><dd> B = Next Slot Number.<br>
HL = Pointer to the next work area.</dd>
<dt>Modifies: </dt><dd>       ?</dd>
</dl>

### Function 2:
<dl>
<dt>Use:</dt><dd>     Disabling interrupts.</dd>
<dt>Entry:</dt><dd>   Nothing</dd>
<dt>Output:</dt><dd>  Nothing</dd>
<dt>Modifies:</dt><dd>        Nothing</dd>
</dl>

### Function 3:
<dl>
<dt>Use:</dt><dd>     Enabling interrupts.</dd>
<dt>Entry:</dt><dd>   Nothing</dd>
<dt>Output:</dt><dd>  Nothing</dd>
<dt>Modifies:</dt><dd>        Nothing</dd>
</dl>
