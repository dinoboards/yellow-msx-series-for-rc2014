
; CHPUT			EQU	$00A2	; CHARACTER OUTPUT

	EXTERN _chput

	PUBLIC _print_uint16
	; HL = unsigned 16 bit number to write out
	; call CHPUT to write a single ascii character (in A)
_print_uint16:
	ld	e, 0
	ld	bc, -10000
	call	num1
	ld	bc, -1000
	call	num1
	ld	bc, -100
	call	num1
	ld	c, -10
	call	num1
	ld	c, b

num1:	ld	a, '0'-1
num2:	inc	a
	add	hl, bc
	jr	c, num2
	sbc	hl, bc

	cp	'0'
	jr	nz, num3

	ld	a, e
	cp	1
	ret	nz
	ld	a, '0'

num3:
	ld	e, 1
	jp	_chput2

_chput2:
	push	af
	push	hl
	push	bc
	ld	l, a
	call	_chput
	pop	bc
	pop	hl
	pop	af
	ret
