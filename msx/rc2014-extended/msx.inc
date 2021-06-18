
	include	"../cbios/src/hardware.asm"
	include	"../cbios/src/hooks.asm"
	include	"../cbios/src/systemvars.asm"

NMI		EQU	$0066

; MSX 1 BIOS Entries
; --------------------------------------------------------
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
OLDINT:		EQU	$FB0C		; ORIGINAL IKEYI HOOK HANDLER
MEXBIH:		equ	$FB07		; ORIGINAL EXTBIO HOOK JUMP FUNCTION
HOKVLD:		EQU	$FB20		; BIT 0 - EXTENDED BIOS INSTALLED


BIOS_FN	.MACRO	ADDR
	LD	IX, &ADDR
	LD	IY, (EXPTBL-1)		; BIOS SLOT IN IYH
		CALL	CALSLT			; INTERSLOT CALL
	.ENDM

