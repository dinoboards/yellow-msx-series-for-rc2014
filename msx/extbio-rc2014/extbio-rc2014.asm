
	ORG	08000H

	include "msx.inc"

; +0	ID		Put these first two bytes at 041H and 042H ("AB") to indicate that it is an additional ROM.
; +2	INIT		Address of the routine to call to initialize a work area or I/O ports, or run a game, etc.
; +4	STATEMENT	Runtime address of a program whose purpose is to add instructions to the MSX-Basic using CALL.
; +6	DEVICE		Execution address of a program used to control a device built into the cartridge. For example, a disk interface.
; +8	TEXT		Pointer of the tokenizen Basic program contained in ROM.
; +10	Reserved	6 bytes reserved for future updates.

ROM_HEADER:
	DB	"AB"
	DW	ROM_INIT
	DW	0
	DW	0
	DW	0
	DS	6

ROM_INIT:
	RET

	ORG	08100H

	; CALLED FROM MSX-DOS RC2014 DISK DRIVER
	; HL CONTAINS THE WORKING SPACE PTR
MSXDOS_RC2014_INIT:
	JP	PROBE_HARDWARE

PROBE_HARDWARE:
	; LD	DE, MSG.CPU_SPEED
	; CALL	PRINT

	; LD	DE, MSG.MEMORY
	; CALL	PRINT

	; LD	DE, MSG.VIDEO
	; CALL	PRINT

	LD	DE, MSG.RTC
	CALL	PRINT

	CALL	RP5RTC_PROBE
	JR	Z, RTC_FOUND

	LD	DE, MSG.NOT
	CALL	PRINT

RTC_FOUND:
	LD	DE, MSG.PRESENT
	CALL	PRINT

	; LD	DE, MSG.RTC
	; CALL	PRINT

	; LD	DE, MSG.GAME
	; CALL	PRINT

	; LD	DE, MSG.KEYBOARD
	; CALL	PRINT

	; LD	HL, 6
	; CALL	ALLOC

	RET

MSG.CPU_SPEED
	DB	"CPU:                 ", 9, "7Mhz", 13, 10, 0

MSG.MEMORY
	DB	"Memory Module:       ", 9, "512K, 1024K", 13, 10, 0

MSG.RTC
	DB	"RTC Module:          ", 9, 0

MSG.GAME
	DB	"Game Module:         ", 9, "PRESENT", 13, 10, 0

MSG.VIDEO
	DB	"Video Module:        ", 9, "V9958", 13, 10, 0

MSG.KEYBOARD
	DB	"PPI/Keyboard Module: ", 9, "PRESENT", 13, 10, 0

MSG.PRESENT
	DB	"PRESENT", 13, 10, 0

MSG.NOT
	DB	"NOT ", 0

	INCLUDE "alloc.asm"
	INCLUDE	"utils.asm"

;
; DETECT RTC HARDWARE PRESENCE
; RTC HARDWARE IS NORMALL MANAGED BY THE BIOS
; SO WE ONLY NEED TO TEST IF PRESENT.
;
RP5RTC_PROBE:
	LD	C, 00100000b
	LD	IX, REDCLK
	CALL	CALSUB
	AND	0Fh
	CP	0Ah
	RET

; CALSUB
;
; In: IX = address of routine in MSX2 SUBROM
;     AF, HL, DE, BC = parameters for the routine
;
; Out: AF, HL, DE, BC = depending on the routine
;
; Changes: IX, IY, AF', BC', DE', HL'
;
; Call MSX2 subrom from MSXDOS. Should work with all versions of MSXDOS.
;
; Notice: NMI hook will be changed. This should pose no problem as NMI is
; not supported on the MSX at all.
;
CALSUB:	EXX
	EX	AF, AF'			; STORE ALL REGISTERS
	LD	HL, EXTROM
	PUSH	HL
	LD	HL, 0C300h
	PUSH	HL			; PUSH NOP ; JP EXTROM
	PUSH	IX
	LD	HL, 021DDh
	PUSH	HL			; PUSH LD IX, <ENTRY>
	LD	HL, 03333h
	PUSH	HL			; PUSH INC SP; INC SP
	LD	HL, 0
	ADD	HL, SP			; HL = OFFSET OF ROUTINE
	LD	A, 0C3h
	LD	(H_NMI), A
	LD	(H_NMI+1), HL		; JP <ROUTINE> IN NMI HOOK
	EX	AF, AF'
	EXX				; RESTORE ALL REGISTERS
	LD	IX, NMI
	LD	IY, (EXPTBL-1)
	CALL	CALSLT			; CALL NMI-HOOK VIA NMI ENTRY IN ROMBIOS
					; NMI-HOOK WILL CALL SUBROM
	EXX
	EX	AF, AF'	  		; STORE ALL RETURNED REGISTERS
	LD	HL, 10
	ADD	HL, SP
	LD	SP, HL	  		; REMOVE ROUTINE FROM STACK
	EX	AF, AF'
	EXX				; RESTORE ALL RETURNED REGISTERS
	RET


; REDCLK
; Address  : #01F5
; Function : Read clock-RAM
; Input    : C  - clock-RAM address
;                 xxBBAAAA
;                   ||++++-- address
;                   ++------ Block-number
; Output   : A  - Read value in lowest four bits
; Registers: F
; WRTCLK
; Address  : #01F9
; Function : Write clock-RAM
; Input    : C  - clock-RAM address
;                 xxBBAAAA
;                   ||++++-- address  :
;                   ++------ Block-number
;            A  - Value to write
; Registers: F

	DS	$C000 - $

; FB03	RSTMP	50		Used by RS-232C or optical
; FB03	TOCNT	1		Used internally by RS-232C
; FB04	RSFBC	1		RS-232C LOW address
; 1		High address of RS-232C
; FB06	RSIQLN	1		Used internally by RS-232C
; FB07	MEXBIH	Five		FB07H + 0 RST 30H (0F7H)
; FB07H + 1 byte data
; FB07H + 2 (LOW)
; FB07H + 3 (HIGH)
; FB07H + 4 RET (0C9H)
; FB0C	OLDSTT	Five		FB0CH + 0 RST 30H (0F7H)
; FB0CH + 1 byte data
; FB0CH + 2 (LOW)
; FB0CH + 3 (HIGH)
; FB0CH + 4 RET (0C9H)
; FB11	OLDINT	Five		FB11H + 0 RST 30H (0F7H)
; FB11H + 1 byte data
; FB11H + 2 (LOW)
; FB11H + 3 (HIGH)
; FB11H + 4 RET (0C9H)
; FB16	DEVNUM	1		Used internally by RS-232C
; FB17	DATCNT	3		FB17H + 0 byte data
; FB17H + 1 byte pointer
; FB17H + 2 byte pointer
; FB1A	ERRORS	1		Used internally by RS-232C
; FB1B	FLAGS	1		Used internally by RS-232C
; FB1C	ESTBLS	1		Used internally by RS-232C
; FB1D	COMMSK	1		Used internally by RS-232C
; FB1E	LSTCOM	1		Used internally by RS-232C
; FB1F	LSTMOD	1		Used internally by RS-232C
; FB20	HOKVLD	1		With or without extended BIOS
; FB21	DRVTBL	8		DISK ROM slot address, etc.
