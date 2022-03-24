
	include	"msx.inc"

;-----------------------------------------------------------------------------
;
; Obtain the work area address for the main st driver portion
; Input: A=1  to obtain the work area for the master, 2 for the slave
; output: HL WORK AREA ADDRESS
; Preserves A
	PUBLIC	_get_work_area
_get_work_area:
	PUSH	IX
	XOR	A
	EX	AF,AF'
	XOR	A
	LD	IX, GWORK
	CALL	CALBNK
	ld	l, (IX)
	ld	h, (IX+1)
	POP	IX
	RET
