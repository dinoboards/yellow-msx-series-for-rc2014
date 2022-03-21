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

	; COPY NXT_DIRECT TO WORK-AREA
	JP	NXT_DIRECT_WRKAREA


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

ADD_512_TO_HL:
	push	de
	ld	de, 512
	add	hl, de
	pop	de
	ret

	; 1 to 32bit number at DE
INC_SECTOR_NUMBER:
	push	bc
	push	de
	push	hl
	ex	de, hl

	push	hl
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	inc	hl
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	pop	hl
	inc	c
	jr	NZ,l_increment32Bit_00103
	inc	b
	jr	NZ,l_increment32Bit_00103
	inc	de
l_increment32Bit_00103:
	ld	(hl), c
	inc	hl
	ld	(hl), b
	inc	hl
	ld	(hl), e
	inc	hl
	ld	(hl), d

	pop	hl
	pop	de
	pop	bc
	ret

DEV_WRT_USB:
	push	iy
	push	af			; reserve 4 bytes for modifying sector number
	push	af
	ld	iy, 0
	add	iy, sp

	ld	a, (de)
	inc	de
	ld	(iy), a
	ld	a, (de)
	inc	de
	ld	(iy+1), a
	ld	a, (de)
	inc	de
	ld	(iy+2), a
	ld	a, (de)
	inc	de
	ld	(iy+3), a
	push	iy
	pop	de

DEV_WRT_USB_NXT_SECTOR:

	LD	A, 2
	PUSH	AF

DEV_WRT_SECTOR_LOOP:
	call	CAPS_FLASH
	push	bc
	ld	b, 1
	call	SAFE_SCSI_WRITE
	pop	bc
	jr	nc, _DEV_RW_NEXT_4

	POP	AF
	DEC	A
	JR	Z, _DEV_RW_ERR

	PUSH	AF
	push	bc

	ld	bc, 60*3
	ei
	call	WAIT
	pop	bc

	jr	DEV_WRT_SECTOR_LOOP

_DEV_RW_NEXT_4:
	POP	AF
	call	ADD_512_TO_HL
	call	INC_SECTOR_NUMBER
	djnz	DEV_WRT_USB_NXT_SECTOR


	pop	af			; restore stack and iy
	pop	af
	pop	iy

	; CAPS OFF
	in	a, 0xaa
	set	6, a
	out	0xaa, a
	xor	a ; success
	ret

_DEV_RW_ERR:
	call	CAPS_FLASH

	pop	af			; restore stack and iy
	pop	af
	pop	iy

	ld	a, _RNF
	ld	b, 0
	ret

DEV_READ_USB:
	push	iy
	push	af			; reserve 4 bytes for modifying sector number
	push	af
	ld	iy, 0
	add	iy, sp

	LD	A, 2
	PUSH	AF

DEV_READ_USB_TRY_AGAIN:
	call	CAPS_FLASH
	call	SAFE_SCSI_READ

	jr	nc, _DEV_RW_NEXT_4
	POP	AF
	DEC	A
	JR	Z, _DEV_RW_ERR

	push	af
	push	bc

	ld	bc, 60*3
	ei
	call	WAIT
	pop	bc
	jr	DEV_READ_USB_TRY_AGAIN

CAPS_FLASH:
	in	a, (0xaa)
	bit	6, a
	jr	z, _CAPS_FLASH_ON
	res	6, a
    	jr	_CAPS_FLASH

_CAPS_FLASH_ON:
	set	6, a

_CAPS_FLASH:
	out	(0xaa), a
    	ret

SAFE_SCSI_WRITE:
	push	iy
	push	ix
	push	hl
	push	de
	push	bc
	call	SCSI_WRITE
	pop	bc
	pop	de
	pop	hl
	pop	ix
	pop	iy
	ret

SAFE_SCSI_READ:
	push	iy
	push	ix
	push	hl
	push	de
	push	bc
	call	SCSI_READ
	pop	bc
	pop	de
	pop	hl
	pop	ix
	pop	iy
	ret
