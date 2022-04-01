DRV_INIT_CH376:
	push	ix
	call	_usb_host_init
	pop	ix

	ld	a, l
	or	a
	ret	z

	OR	255
	RET

; ------------------------------------------------
; Initialize the Work-area
; ------------------------------------------------
INIWORK:
	; CLEAR THE WORKAREA
	LD	HL, IX
	LD	DE, HL
	INC	DE
	LD	BC, ST_WRKAREA-1 ; SIZE OF ST & USB WRKAREA
	XOR	A
	LD	(HL),A
	LDIR
	RET


; USB-STORAGE DRIVER FUNCTIONS

DEV_INFO_USB:
	push	hl
	push	bc
	inc	sp
	call	_usb_dev_info
	pop	af
	inc	sp
	ld	a, l
	RET

LUN_INFO_USB:
	push	hl
	push	bc
	inc	sp
	call	_usb_lun_info
	pop	af
	inc	sp
	ld	a, l
	RET


DEV_STATUS_USB:
	ld	l, b
	call	_usb_dev_status
	ld	a, l
	ret

DEV_WRT_USB:
	push	ix
	dec	sp

	ld	ix, 0
	add	ix, sp
	push	ix			; push address for return value

	push	hl			; push buffer

	ex	de, hl
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	ld	h, (hl)
	ld	l, a			; hlbc is sector number

	push	hl			; sector number
	push	de			; sector number
	push	bc			; push lun and sector count

	call	_usb_dev_write
	pop	af			; restore stack
	pop	af
	pop	af
	pop	af
	pop	af

	ld	a, l			; return values
	ld	b, (ix-1)

	inc	sp			; restore stack
	pop	ix
	ret



DEV_READ_USB:
	push	ix
	dec	sp

	ld	ix, 0
	add	ix, sp
	push	ix			; push address for return value

	push	hl			; push buffer

	ex	de, hl
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
	ld	a, (hl)
	inc	hl
	ld	h, (hl)
	ld	l, a			; hlbc is sector number

	push	hl			; sector number
	push	de			; sector number
	push	bc			; push lun and sector count

	call	_usb_dev_read
	pop	af			; restore stack
	pop	af
	pop	af
	pop	af
	pop	af

	ld	a, l			; return values
	ld	b, (ix-1)

	inc	sp			; restore stack
	pop	ix
	ret

