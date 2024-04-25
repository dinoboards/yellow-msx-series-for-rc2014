
	include	"../cbios/src/hardware.asm"
	include	"../cbios/src/hooks.asm"
	include	"../cbios/src/systemvars.asm"

NMI		EQU	$0066


; MSX 1 BIOS Entries
; --------------------------------------------------------

; Function : Reads the value of an address in another slot
; Input    : A  - ExxxSSPP  Slot-ID
;            │        ││└┴─ Primary slot number (00-11)
;            │        └┴─── Secondary slot number (00-11)
;            └───────────── Expanded slot (0 = no, 1 = yes)
;            HL - Address to read
; Output   : A  - Contains the value of the read address
; Registers: AF, C, DE
; Remark   : This routine turns off the interupt, but won't turn it on again
RDSLT		EQU	$000C

; Function : Writes a value to an address in another slot.
; Input    : A  - Slot ID, see RDSLT
;            HL - Address
;            E  - Value
; Registers: AF, BC, D
; Remark   : See RDSLT
WRSLT		EQU	$0014

; Function : Displays one character
; Input    : A  - ASCII code of character to display
CHPUT		EQU	$00A2		; Displays one character

; Function : Reads the primary slot register
; Output   : A  - For the value which was read
;            33221100
;            ││││││└┴─ Page 0 (#0000-#3FFF)
;            ││││└┴─── Page 1 (#4000-#7FFF)
;            ││└┴───── Page 2 (#8000-#BFFF)
;            └┴─────── Page 3 (#C000-#FFFF)
; Registers: A
RSLREG		EQU	$0138

; Function : Executes inter-slot call.
; Input    : IY - High byte with slot ID, see RDSLT
;            IX - The address that will be called
; Remark   : Variables can never be given in alternative registers or IX and IY
CALSLT		EQU	$001C

; Function : Tests status of CTRL-STOP
; Output   : Carry flag set when pressed
; Registers: AF
; Remark   : In this routine, interrupts are inhibited
BREAKX		EQU	$00B7

; MSX 2 BIOS Entries
; --------------------------------------------------
; Function : Calls a routine in SUB-ROM. Most common way
; Input    : IX - Address of routine in SUB-ROM
; Output   : Depends on the routine
; Registers: Alternative registers, IY
; Remark   : Use: LD IX,address
;                 CALL EXTROM
EXTROM		EQU	$015F

; MSX 2 SUBROM BIOS Entries
; -----------------------------

; Function : Read clock-RAM
; Input    : C  - clock-RAM address
;                 xxBBAAAA
;                   ||++++-- address
;                   ++------ Block-number
; Output   : A  - Read value in lowest four bits
; Registers: F
REDCLK		EQU	$01F5


; RS232 SYSTEM VARS/HOOKS
RS_TMP:		EQU	$FB03		; (1 BYTE)  TEMPORARY DATA STORAGE FOR RS232 DRIVER
RS_FCB:		EQU	$FB04		; (2 BYTES) FCB ("FILE CONTROL BLOCK") ADDRESS OF THE RS-232C
RS_IQLN:	EQU	$FB06		; (1 BYTE)  BYTE DATA USED BY THE RS-232C INTERFACE - SIZE OF BUFFER
RS_MEXBIH:	EQU	$FB07		; (5 BYTES) ORIGINAL EXTBIO HOOK JUMP FUNCTION
RS_OLDSTT:	EQU	$FB0C		; (5 BYTES) REPURPOSED - SAVE FOR H.NEWS
RS_OLDINT:	EQU	$FB11		; (5 BYTES) ORIGINAL IKEYI HOOK HANDLER
RS_DEVNUM:	EQU	$FB16		; (1 BYTE)  MSX SERIAL DEVICENUMBER/TRAP NUMBER
RS_DATCNT:	EQU	$FB17		; (1 BYTE)  REPURPOSED - CURRENT BUFFER COUNT
RS_BUFEND:	EQU	$FB18		; (2 BYTES) REPURPOSED - CURRENT BUFFER END PTR
RS_ERRORS:	EQU	$FB1A		; (1 BYTE)  RS-232C ERROR CODE
RS_FLAGS:	EQU	$FB1B		; (1 BYTE)  RS-232C RS_FLAGS (BIT 1 - RTS ON, BIT 3 - SET IF OPENED)
RS_ESTBLS:	EQU	$FB1C		; (1 BYTE)  BIT BOOLEAN. (RS-232C)
RS_LSTCOM:	EQU	$FB1E		; (1 BYTE)  REPURPOSED - BYTE DATA. (RS-232C)
RS_LSTMOD:	EQU	$FB1F		; (1 BYTE)  REPURPOSED - BYTE DATA. (RS-232C)
RS_HOKVLD:	EQU	$FB20		; (1 BYTE)  BIT 0 - EXTENDED BIOS INSTALLED

RS_P1_SEG:	EQU	$FB03		; RE-PURPOSE RS_TMP - SEGMENT NUMBER ALLOCATED FOR USE BY FOSSIL DRIVER

RS_SIO_A_CMD:	EQU	$FB0C		; THE SIO/2 COMMAND PORT
RS_SIO_A_DAT:	EQU	$FB0D		; THE SIO/2 DATA PORT
RS_SIO_B_CMD:	EQU	$FB0E		; THE SIO/2 COMMAND PORT
RS_SIO_B_DAT:	EQU	$FB0F		; THE SIO/2 DATA PORT

RS_SIO_CLK_DIV:	EQU	$FB1E		; RE-PURPOSE RS_LSTCOM - BIT 6 AND 7 - SIO/2 CLOCK DIVIDER SETTING (/16, /32 OR /64) AS PER REGISTER 4


SIO_CLK_307200	EQU	1	; Baud rates of  (4800    9600    19200)
SIO_CLK_614400	EQU	2	; Baud rates of  (9600    19200   38400)
SIO_CLK_921600	EQU	3	; Baud rates of  (14400   28800   57600)
SIO_CLK_1228800	EQU	4	; Baud rates of  (19200   38400   76800)
SIO_CLK_2457600	EQU	5	; Baud rates of  (38400   76800   153600)
SIO_CLK_3686400	EQU	6	; Baud rates of  (57600   115200  230400)

; RE-PURPOSE RS_LSTMOD - NUMBER INDICATING SIO/2 CLOCK SIGNAL
RS_SIO_CLK	EQU	$FB1F

BIOS_FN	MACRO	ADDR
	LD	IX, ADDR
	LD	IY, (EXPTBL-1)		; BIOS SLOT IN IYH
	CALL	CALSLT			; INTERSLOT CALL
	ENDM

; ; SLTWRK+124 = Work area for slot 3-3, page 2
; WORK   EQU     SLTWRK+124
WORK   EQU     SLTWRK+(3*32+3*8+2*2)


; FOSSIL VARIABLES

; I GOT CAUGHT WITH THE FOSSIL DOCUMENTATION BEING AMBIGUOUS AS TO
; WHERE THE MARKER/JUMP TABLE ADDRESSES ARE TO BE STORED.
; SEE UPDATED DOC msx/rc2014-extended/docs/fossilspec.md FOR MORE DETAILS.
; TO ENSURE EXISTING RC2014 UTILITIES CONTINUE TO WORK WE NEED TO
; CONTINUE TO SUPPORT THE OLD MARKERS.  A NEW VERSION OF THE EXTBIO
; FUNCTION API IS PROVIDED TO INSTALL FOSSIL FOR SIO INTO THE CORRECT
; ADDRESSES

FSMARK_INCORRECT	EQU	$F3FE		; CONTAINS "RS" IF INSTALLED AND ACTIVE
FSTABL_INCORRECT	EQU	$F400		; FOSSIL JUMP TABLE ADDRESS

FSMARK	EQU	$F3FC		; CONTAINS "RS" IF INSTALLED AND ACTIVE
FSTABL	EQU	$F3FE		; FOSSIL JUMP TABLE ADDRESS

MEMMAP_TBL	EQU	0F200H
MEMMAP_ALLOC	EQU	MEMMAP_TBL + 0
MEMMAP_FREE	EQU	MEMMAP_TBL + 3
MEMAP_PUT_P1	EQU	MEMMAP_TBL + 1EH
MEMAP_GET_P1	EQU	MEMMAP_TBL + 21H
MEMAP_PUT_P2	EQU	MEMMAP_TBL + 24H
MEMAP_GET_P2	EQU	MEMMAP_TBL + 27H

// SUPER HDMI FLAGS
SUPF1	EQU	0FFF8H	; BIT 7 = 1 - SUPER HDMI FOUND AND ENABLED
SUPF2	EQU	0FFF9H	; BIT 0 = 1 - SUPER MODE ON, BITS 2:1 -> SUPER MODE (00: COLOR, 01: MID, 10: HIGH, 11: NOT_USED)
