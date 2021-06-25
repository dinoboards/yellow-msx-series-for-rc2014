

	PUBLIC	_intOn, _intOff, _counter, _period, _intTriggered

	SECTION	CODE

	; TODO COPY TO HIGHMEM OR CHANGE INT HANDLER TO DO SLTCAL

	defs	$8000 - ASMPC

TIMI    EQU     0FD9FH          ;timer interrupt hook
JPCODE  EQU     0C3H

;----- interrupt on ----- Note:  restore the former hook when changing the hook

_intOn:
INTON:  DI
        LD      HL,TIMI         ;OLD HOOK SAVE
        LD      DE,HKSAVE
        LD      BC,5
        LDIR

        LD      A,JPCODE        ;NEW HOOK SET
        LD      (TIMI),A
        LD      HL,INTHANDL
        LD      (TIMI+1),HL
        EI
        RET

;----- interrupt off ----- Note:  restore the reserved hook and exit

_intOff:
INTOFF: DI
        LD      HL,HKSAVE
        LD      DE,TIMI
        LD      BC,5
        LDIR
        EI
        RET

;----- interrupt routine -----

INTHANDL:
	PUSH    AF
	PUSH	HL
	LD	HL, (_counter)
	LD	(_period), HL
	LD	A, 255
	LD	(_intTriggered), A

	POP	HL
FIN:    POP     AF
HKSAVE: NOP                     ;old HOOK save area
        NOP
        NOP
        NOP
        RET

        ; JP    HKSAVE          ;old HOOK call

        ; RET

	SECTION	DATA

_counter:	DW	0
_period: 	DW	0
_intTriggered:	DB	0

