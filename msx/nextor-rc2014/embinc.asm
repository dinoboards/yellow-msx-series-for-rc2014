
DRV_BANK_NO	equ	7

SECCNT	EQU	30		; NUMBER OF SECTORS STORED IN EACH BANK -- this can be increased?
SECSIZ	EQU	512		; NUMBER OF BYTES PER SECTOR
SECTOT	EQU	18*SECCNT
SECBNK	EQU	DRV_BANK_NO+1	; BANK ID OF FIRST SECTOR

PRES_CF		EQU	1	; BIT MASK FOR COMPACTFLASH PRESENT
PRES_MS		EQU	2	; BIT MASK FOR MSX MUSIC NOR FLASH PRESENT
PRES_USB	EQU	4	; BIT MASK FOR USB STORAGE PRESENT AT BOOT UP
BIT_PRES_CF	EQU	0	; BIT POSTION FOR COMPACTFLASH PRESENT
BIT_PRES_MS	EQU	1	; BIT POSTION FOR MSX MUSIC NOR FLASH PRESENT
BIT_PRES_USB	EQU	2	; BIT POSTION FOR USB STORAGE PRESENT


DEV_MAP_NONE	EQU	0
DEV_MAP_ROM	EQU	1
DEV_MAP_CF	EQU	2
DEV_MAP_MS	EQU	3
DEV_MAP_USB1	EQU	4
DEV_MAP_USB2	EQU	5
DEV_MAP_USB3	EQU	6
DEV_MAP_USB4	EQU	7

;* Get in IX the address of the SLTWRK entry for the slot passed in A,
;  which will in turn contain a pointer to the allocated page 3
;  work area for that slot (0 if no work area was allocated).
;  If A=0, then it uses the slot currently switched in page 1.
;  Returns A=current slot for page 1, if A=0 was passed.
;  Corrupts F.
;  Must be called by using CALBNK to bank 0:
;    ld a,<slot number> (xor a for current page 1 slot)
;    ex af,af'
;    xor a
;    ld ix,GWORK
;    call CALBNK

GWORK	equ	4045h

;-----------------------------------------------------------------------------
;
; Routines and information available on kernel page 0
;

;* Get in A the current slot for page 1. Corrupts F.
;  Must be called by using CALBNK to bank 0:
;    xor a
;    ld ix,GSLOT1
;    call CALBNK

GSLOT1	equ	402Dh

;* Call a routine in another bank.
;  Must be used if the driver spawns across more than one bank.
;
;  Input:  A = bank number
;          IX = routine address
;          AF' = AF for the routine
;          HL' = Ix for the routine
;          BC, DE, HL, IY = input for the routine
;  Output: AF, BC, DE, HL, IX, IY returned from the called routine.

CALBNK	equ	4042h

