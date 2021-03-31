
RAMAD0	equ	0F341h	; Slot of the Main-Ram on page 0~3FFFh
extbio	equ	0ffcah	; Call address to an extended Bios

	SECTION	CODE
	PUBLIC	_getdev, _table, _getsystem

_getdev:
	ld	hl, _table
	call	getslt	; B = number the slot of the table
	ld	d,0	; Broad-cast device
	ld	e,0	; Function 'get device number'
	jp	extbio

_getsystem:
	ld	hl, _table
	call	getslt	; B = number the slot of the table
	xor	a
	ld	d,255	; system????
	ld	e,0	; Function 'get'
	jp	extbio

; Routine getslt
; Entry: HL = Address in Ram
; Output: B = Slot number corresponding to the HL address
; Modifies: A and B

getslt:
	push	hl
	ld	a,h
	rla
	rla
	rla		; Bit 6-7 to bit 1-0
	and	3	; Reset unused bits
	ld	c,a
	ld	b,0
	ld	hl,RAMAD0
	add	hl,bc
	ld	b,(hl)	; B = Slot Number of Main RAM
	pop	hl
	ret

; ;
; GETENT --Get entry address
; Entry: none
; Return: carry flag is set if no devices
; Modify: all
; ;
; device equ 8; device # 8 is RS-232C
; getent:
;         ld bc, table
;         call getsit; get slot address of TABLE
;         ld h, b
;         ld l, c
;         ld b, a
;         ld d, device; set deice number
;         ld e, 0;'get entry address' command
;         push hl; save TABLE address
;         call extbio
;         pop de; restore TABLE address
;         or a
;         sbc hl, de; how many devices?
;         ret nz; there are some devices
;         scf; no devices
;         ret
; table:
;         ds 32 * 4; assume maximum 32 devices


_table:
	ds	32	; Reserve 32 bytes for the table
