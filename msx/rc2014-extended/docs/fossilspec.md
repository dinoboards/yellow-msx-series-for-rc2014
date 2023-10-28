Source: https://www.msx.org/wiki/Fossil_driver

This document describes how to use the RS232C fossil driver created by Erik Maas

First some nice things to know about this driver:

* It automatically tries to detect the RS232C interface
* It can receive data up to a rate of 115200 baud
* To do the highest possible rates, the driver can install itself at
  hook &h0038, if there is RAM at this location. (e.g. DOS)
* If you write software for one type of interface, it should automatically work
  on the other interfaces too!


To use this driver, you have to do the following things:

* Detect the driver
	Check for the "RS" mark at address ~~&HF3FE~~ &HF3FC
* Get the driver jump table
	The address of the jump table is at ~~&HF3FE+2=&HF400~~  &HF3FE

> **The above points seem to be inconsistent with the sample code below
> The RC2014 implementation had incorrectly used the original incorrect addresses
> for the marker and jump table - creating incompatibilities with RC2014 utilities and
> general MSX utilities.**

In MLFOSSIL.GEN you see an example of how to use this jump table.

From here on, more detailed information follows, or better, something like
a copy from sources...


```
&HF3FC: DB	"RS"
	DW	DRIVER

DRIVER:
	jp	getversion	; Version H.L (H,L packed BCD)
	jp	init		; initialises RS232
	jp	deinit		; deinitialises the RS232

	jp	setbaud		; H=Tx baud rate, L=Rx baud rate
				; 0  = 75	 1  = 300	  2  = 600
				; 3  = 1200	 4  = 2400	  5  = 4800
				; 6  = 9600	 7  = 19200	  8  = 38400
				; 9  = 57600	 11 = 115200

; Extra information :
;	In register H and L, the driver reports the actually selected rate.
;	This is done in case the hardware does not support the selected
;	rate. If the driver does not support the selected rate, it selects
;	the highest possible rate below the selected.

	jp	protocol	; H 0-1 data bits
				;	00 5 bits or less
				;	01 6 bits
				;	10 7 bits
				;	11 8 bits
				;   2-3 stop bits
				;	01 1 stopbit
				;	10 1.5 stopbits
				;	11 2 stopbits
				;   4-5 parity
				;	00 none
				;	01 even
				;	11 odd
				;   6-7 0
				; L = 0 (has to be set to zero, for future
				;	 extensions!)
	jp	channel 	; H	channel number (0-1) (NMS only)

	jp	rs_in		; read from RS232 buffer, A = character/byte
	jp	rs_out		; write to RS232, A = character/byte

	jp	rs_in_stat	; A=0 No data in buffer, A!=0 data in buffer
				; The F register is set according the result
	jp	rs_out_stat	; At this time every time=!0, but this is
				; reserved for future expansion of the driver
				; to sent data on interrupt. This function
				; is not needed to use, rs_in_stat is required
				; before rs_in is done.

	jp	dtr		; H=0 drop DTR, H=255 raise DTR
	jp	rts		; H=0 drop RTS, H=255 raise RTS

	jp	carrier 	; A=0 no carrier, A!= carrier detect

	jp	chars_in_buf	; Return : HL = characters in Rx buf.
	jp	size_of_buf	; Return : HL = size of buffer
	jp	flushbuf	; FLUSH THE RECEIVE BUFFER

	jp	fastint		; use &H0038 hook for speedup
				; H=0 Connect driver fast
				; H=1 Release fast hook

; This fastint needs some extra attention:
; The driver uses hook &HFD9A, but this is not very fast. Since this hook
; is called after some BIOS work, and after this hook, the BIOS continues
; to do some time waisting things.
; Therefore, this driver has the option to install itselve at the &H38 hook.
; This can be done if there is RAM available at &H0000-&H3FFF. This is the
; case in DOS(2).

	jp	hook38stat	; set status for 0038 hook
				; h  = 0, every interrupt is supported
				; h != 0, only RS232 interrupt (and VDP)

; This function is for use with fastint.
; When the "connect driver fast" has been issued, you can control how the
; interrupt handler of the driver behaves.
; If every interrupt is supported, there is slightly more processor time
; used after an interrupt from the RS232C interface.

	jp	chput_hook	; redirect CHPUT data to RS232
				; H=0 no redirection
				; H=1 redirect with echo
				; H=3 redirect without echo (faster)

; All print commands that would be issued using the BIOS calls, will be
; transmitted through the RS232C interface.

	jp	keyb_hook	; redirect RS232 to keyboard buffer
				; H=0 release hook, H!=0 bend hook

; With this function, you can redirect all incoming RS232C data to the
; keyboard buffer.
; This and the previous function are nice to let a terminal control your
; MSX computer.

	jp	get_info	; Gives a pointer in HL that points to a
				; driver info block. This block describes
				; the current situation about the driver.


;************** Driver Info Block **************
;
;	Offset	Bytes	Description
;	+0	2	Version number +1 contains main version
;			+0 contains sub-version, both packed BCD
;	+2	1	Current receive speed index
;	+3	1	Current send speed index
;	+4	1	Current protocol
;			bit 0-1 Data Bits
;				00 5 bits or less
;				01 6 bits
;				10 7 bits
;				11 8 bits
;			    2-3 Stop Bits
;				00 (SYNC modes enable)
;				01 1 stopbit
;				10 1.5 stopbits
;				11 2 stopbits
;			    4-5 Parity
;				00 none
;				01 even
;				11 odd
;			    6-7 0 (not defined yet)
;	+5	1	ChPut_Hook redirection status
;	+6	1	Keyboard_Hook redirection status
;	+7	1	Current RTS status (Request To Send, or NOT!)
;	+8	1	Current DTR status (Data Terminal Ready)
;	+9	1	Current channel
;	+10	1	Hardware info
;			0 = No description
;			1 = ASCII monochannel compatible
;			2 = NMS121x interface (multichannel)
;			4 = NMS1250, adapted for RS232C
;			8 = 16550 UART with FIFO (Sunrise RS232C + ACCNET)
; ************** End of InfoBlock **************


MLFOSSIL.GEN:

;
; RS232 recognize/caller routine
;
; (p) 1993,1994 Erik Maas
;
; made to use with GEN80 macro assembler from HiSoft's DevPack80 v2.03

; To use this library, first you have to call GetFossil, this routine will
; check whether the fossil is available or not, if it is available, then it
; will adjust the jump table, so you can use this table.
; The return flags of GetFossil can tell you whether the fossil was found or
; not. Z  means the Fossil was found and installed
;      NZ means the Fossil was not found, and therefore NOT installed
;         (so be wise, return to DOS or whatever, do not use the routines)

; The fossil has it marks at the cassette parameter table, I do not believe
; any MSX 2 or turbo-R will use it anymore.... (??????)

; F3FC: "R"     mark 1
; F3FD: "S"     mark 2
; F3FE: LSB     LSB of addres to jump table of fossil driver
; F3FF: MSB     MSB of addres to jump table of fossil driver

GetFossil       ld      a,(#f3fc)       ; get first mark of fossil
                cp      "R"             ; is it the right one?
                ret     nz              ; return if not with NZ flags
                ld      a,(#f3fd)       ; get second mark of fossil
                cp      "S"             ; is it the right one?
                ret     nz              ; return if not with NZ flags

                ; now we know the Fossil is installed, just get the addres
                ; of the jump table now, and connect it to the program

                ld      hl,(#f3fe)      ; get addres of jump table
                ld      de,F_GetVersion ; point to my own table
                ld      bc,21*3         ; number of entry's at this moment
                ldir                    ; make a copy of the table
                call    F_GetVersion
                xor     a               ; set Z flags
                ret                     ; and return to caller

;
; Jump Table to use in ML
;
; This table will be filled in by the GetFossil routine, after that you can
; simply call these addresses in the table
;

F_GetVersion    jp      0       ; 0
F_Init          jp      0       ; 1
F_DeInit        jp      0       ; 2
F_SetBaud       jp      0       ; 3
F_Protocol      jp      0       ; 4
F_Channel       jp      0       ; 5
F_RS_In         jp      0       ; 6
F_RS_Out        jp      0       ; 7
F_RS_In_Stat    jp      0       ; 8
F_RS_Out_Stat   jp      0       ; 9
F_DTR           jp      0       ; 10
F_RTS           jp      0       ; 11
F_Carrier       jp      0       ; 12
F_Chars_In_Buf  jp      0       ; 13
F_Size_Of_Buf   jp      0       ; 14
F_Flush_Buf     jp      0       ; 15
F_Fastint       jp      0       ; 16
F_Hook38Stat    jp      0       ; 17
F_Chput_Hook    jp      0       ; 18
F_Keyb_Hook     jp      0       ; 19
F_Get_Info      jp      0       ; 20
```
