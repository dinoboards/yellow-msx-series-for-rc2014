
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
RS_OLDINT:	EQU	$FB11		; ORIGINAL IKEYI HOOK HANDLER
RS_MEXBIH:	EQU	$FB07		; ORIGINAL EXTBIO HOOK JUMP FUNCTION
RS_HOKVLD:	EQU	$FB20		; BIT 0 - EXTENDED BIOS INSTALLED
RS_DATCNT:	EQU	$FB17		; CURRENT BUFFER COUNT
RS_BUFEND:	EQU	$FB18		; CURRENT BUFFER END PTR
RS_FCB:		EQU	$FB04		; FCB ("FILE CONTROL BLOCK") ADDRESS OF THE RS-232C
RS_IQLN:	EQU	$FB06		; BYTE DATA USED BY THE RS-232C INTERFACE - SIZE OF BUFFER
RS_FLAGS:	EQU	$FB1B		; RS-232C RS_FLAGS (BIT 1 - RTS ON, BIT 3 - SET IF OPENED)
RS_ESTBLS:	EQU	$FB1C		; BIT BOOLEAN. (RS-232C)
RS_ERRORS:	EQU	$FB1A		; RS-232C ERROR CODE
RS_TMP:		EQU	$FB03		; TEMPORARY DATA STORAGE FOR RS232 DRIVER

BIOS_FN	MACRO	ADDR
	LD	IX, ADDR
	LD	IY, (EXPTBL-1)		; BIOS SLOT IN IYH
	CALL	CALSLT			; INTERSLOT CALL
	ENDM

; ; SLTWRK+124 = Work area for slot 3-3, page 2
; WORK   EQU     SLTWRK+124
WORK   EQU     SLTWRK+(3*32+3*8+2*2)


; FOSSIL VARIABLES

FSMARK	EQU	$F3FE		; CONTAINS "RS" IF INSTALLED AND ACTIVE
FSTABL	EQU	$F400		; FOSSIL JUMP TABLE ADDRESS

