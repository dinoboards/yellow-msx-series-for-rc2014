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

CHPUT			EQU	$00A2	; CHARACTER OUTPUT

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
