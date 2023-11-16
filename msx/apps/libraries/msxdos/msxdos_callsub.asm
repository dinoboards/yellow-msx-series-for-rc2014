

	include "msxdos.inc"

	SECTION	CODE
	PUBLIC	CALSUB

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
CALSUB:  EXX
         EX     AF, AF'       ; STORE ALL REGISTERS
         LD     HL, EXTROM
         PUSH   HL
         LD     HL, 0C300H
         PUSH   HL           ; PUSH NOP ; JP EXTROM
         PUSH   IX
         LD     HL, 021DDH
         PUSH   HL           ; PUSH LD IX,<ENTRY>
         LD     HL, 03333H
         PUSH   HL           ; PUSH INC SP; INC SP
         LD     HL, 0
         ADD    HL,SP        ; HL = OFFSET OF ROUTINE
         LD     A, 0C3H
         LD     (H_NMI), A
         LD     (H_NMI+1), HL ; JP <ROUTINE> IN NMI HOOK
         EX     AF, AF'
         EXX                 ; RESTORE ALL REGISTERS
         LD     IX, NMI
         LD     IY, (EXPTBL-1)
         CALL   CALSLT      ; CALL NMI-HOOK VIA NMI ENTRY IN ROMBIOS
                             ; NMI-HOOK WILL CALL SUBROM
         EXX
         EX     AF, AF'       ; STORE ALL RETURNED REGISTERS
         LD     HL, 10
         ADD    HL, SP
         LD     SP, HL        ; REMOVE ROUTINE FROM STACK
         EX     AF, AF'
         EXX                 ; RESTORE ALL RETURNED REGISTERS
         RET
