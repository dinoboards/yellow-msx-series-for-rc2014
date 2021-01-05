
	public	_cleanexit


	include "msx.inc"

	SECTION CODE

_cleanexit:
	; Reset V9958 read register back to default of 0
	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A


	ld	a, 0
	ld	ix, CHGMOD
	ld	iy, (EXPTBL-1)
	call	CALSLT

	ld	ix, CHGCLR
	ld	iy, (EXPTBL-1)
	call	CALSLT

	ld	ix, INIT32
	ld	iy, (EXPTBL-1)
	call	CALSLT

	ld	a, 80
	ld	(LINL40), a
	ld	ix, INITXT
	ld	iy, (EXPTBL-1)
	call	CALSLT

	ld	ix, INIPLT
	call	CALSUB

	JP	0

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
NMI:     EQU    $0066
EXTROM:  EQU    $015f
H_NMI:   EQU    $fdd6
;
CALSUB:  exx
         ex     af,af'       ; store all registers
         ld     hl,EXTROM
         push   hl
         ld     hl,$C300
         push   hl           ; push NOP ; JP EXTROM
         push   ix
         ld     hl,$21DD
         push   hl           ; push LD IX,<entry>
         ld     hl,$3333
         push   hl           ; push INC SP; INC SP
         ld     hl,0
         add    hl,sp        ; HL = offset of routine
         ld     a,$C3
         ld     (H_NMI),a
         ld     (H_NMI+1),hl ; JP <routine> in NMI hook
         ex     af,af'
         exx                 ; restore all registers
         ld     ix,NMI
         ld     iy,(EXPTBL-1)
         call   CALSLT      ; call NMI-hook via NMI entry in ROMBIOS
                             ; NMI-hook will call SUBROM
         exx
         ex     af,af'       ; store all returned registers
         ld     hl,10
         add    hl,sp
         ld     sp,hl        ; remove routine from stack
         ex     af,af'
         exx                 ; restore all returned registers
         ret

	SECTION IGNORE
