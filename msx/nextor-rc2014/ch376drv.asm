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
	LD	A, B
	CP	1
	JR	Z, LUN_INFO_USB_VALID

	LD	A, 1
	RET

LUN_INFO_USB_VALID:
	LD	(HL), 0 			; #0 BLOCK DEVICE
	INC 	HL				; #1
	LD 	(HL), 0				; SECTOR SIZE
	INC	HL				; #2
	LD	(HL), 2				; 512 BYTE SECTOR

	CALL	USB_SCSI_READ_CAPACITY

	INC	HL				; #3
	LD	A, (IY+3)			; WRITE USB CAPACITY TO NEXTOR RESPONSE
	LD	(HL), A

	INC	HL				; #4
	LD	A, (IY+2)
	LD	(HL), A

	INC	HL				; #5
	LD	A, (IY+1)
	LD	(HL), A

	INC	HL				; #6
	LD	A, (IY+0)
	LD	(HL), A

	INC	HL				; #7
	LD	(HL), 00000001b 		; REMOVABLE + NON-READ ONLY + NO FLOPPY

	INC 	HL				; #8
	LD	(HL), 0
	INC	HL				; #9
	LD	(HL), 0 			; CYLINDERS/TRACKS

	INC	HL				; #10
	LD	(HL), 0				; HEADS
	INC	HL				; #11
	LD	(HL), 0				; SECTORS PER TRACK

	LD	A, 0
	RET

; RETRIEVE THE NUMBER OF SECTORS AVAILABLE FOR USB STORAGE
; INPUTS
;   IX - ADDRESS OF USB WORK AREA (MY_GWORK)
; OUTPUTS
;   IY - POINTS TO 32BIT NUMBER OF 512 SECTORS FOR USB STORAGE
USB_SCSI_READ_CAPACITY:
	CALL	MY_GWORK

	PUSH	HL							; PROTECT HL

	LD	A, (IX+WRKAREA.STORAGE_DEVICE_INFO.DEVICE_ADDRESS)	; GET STORAGE ADDRESS
	LD	BC, WRKAREA.SCSI_DEVICE_INFO.BUFFER			; BUFFER OFFSET FROM IX
	ADD	IX, BC							; POINT IX TO BUFFER
	LD	IY, IX							; AND ALSO IY
	LD	B, 0
	LD	C, _SCSI_READ_CAPACITY
	LD	DE, 0x8
	LD	HL, SCSI_READ_CAPACITY
	OR	A							; READ SCSI
	PUSH	IY							; STORE IY
	CALL	DO_SCSI_CMD						; EXECUTE USB/SCSI COMMAND
	POP	IY							; RETRIEVE BUFFER ADDRESS
	JR	NC, USB_SCSI_READ_CAPACITY_OK

	LD	(IY+0), 0
	LD	(IY+1), 0
	LD	(IY+2), 1
	LD	(IY+3), 0

USB_SCSI_READ_CAPACITY_OK:
	POP	HL							; RESTORE REGISTERS
	RET


DEV_STATUS_USB:
	call	MY_GWORK
	; bit 0 = CH376s present,
	; bit 1 = initialised,
	; bit 2 = USB device present,
	; bit 3 = USB device mounted,
	; bit 4 = virtual DSK inserted,
	; bit 5 = DSK changed
	ld a, (ix+WRKAREA.STATUS)
	; DSK present?
	bit 4,a
	jr z, _DEV_STATUS_ERR
	; changed?
	bit 5,a
	jr z, _DEV_STATUS_NO_CHANGE
	res 5,a
	ld (ix+WRKAREA.STATUS),a
	ld a, 2 ; available, changed
	ret

_DEV_STATUS_NO_CHANGE:
	ld a, 1 ; available, no change
	ret

_DEV_STATUS_ERR:
	xor	a ; not available
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

    STRUCT _SCSI_READ_CAPACITY
BASE:			; Offset to the base of the data structure
OPERATION_CODE:		db 0x25
LUN:	            	db 0
RESERVED:		ds 8, 0
PAD:			ds 2, 0
    ENDS

SCSI_READ_CAPACITY          _SCSI_READ_CAPACITY
