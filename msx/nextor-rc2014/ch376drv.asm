DRV_INIT_CH376:
	CALL	USBHOST_INIT

	CALL	CH_RESET
	LD	BC, WAIT_ONE_SECOND/10
	CALL	WAIT

	CALL	CH_HW_TEST
	JR	NC, CH376_FOUND

CH376_NOT_FOUND:
	LD	HL, CH376_NOT_FOUND_MSG
	CALL	PRINT
	XOR	A
	RET

CH376_FOUND:
	; LD	HL, CH376_FOUND_MSG
	; CALL	PRINT

	LD	A, (IX+WRKAREA.STATUS)
	SET	0, A
	LD	(IX+WRKAREA.STATUS), A

	; CHECK AND DISPLAY CH376S IC/FIRMWARE VERSION
	LD	HL, CH376_FOUND_MSG
	CALL	PRINT
	CALL	CH_IC_VERSION
	ADD	A, '0'
	CALL	CHPUT
	LD	HL, CH376_NEWLINE
	CALL	PRINT

	CALL	USB_HOST_BUS_RESET

	; enumerate and initialise USB devices
	PUSH	IX
	CALL	FN_CONNECT
	POP	IX
	OR	A
	JP	NZ, _USB_MODE_OKAY

	LD	HL, USB_NOT_FOUND_MSG
	CALL	PRINT

	XOR	A
	RET

_USB_MODE_OKAY:
	LD	A, (IX+WRKAREA.STATUS)
	SET	1, A
	LD	(IX+WRKAREA.STATUS),A
	LD	A, (IX+WRKAREA.STORAGE_DEVICE_INFO.DEVICE_ADDRESS)
	AND	A
	JR	NZ, _FOUND_
	LD	HL, USB_FLASH_NOT_FOUND_MSG
	CALL	PRINT
	XOR	A
	RET

_FOUND_:
	LD	HL, USB_FLASH_FOUND_MSG
	CALL	PRINT

	call SCSI_MAX_LUNS
	JR	NC, __OK1
	XOR	A
	RET

__OK1:
	call SCSI_INIT
	call SCSI_INQUIRY
	JR	NC, __OK2
	XOR	A
	RET

__OK2:
	ld	a, (ix+WRKAREA.STATUS)
	set	2, a				; usb device present
	ld	(ix+WRKAREA.STATUS), a

_SCSI_TEST_AGAIN:
	call	SCSI_TEST
	jr 	nc, _SCSI_TEST_OKAY
	call	SCSI_REQUEST_SENSE
	jr	_SCSI_TEST_AGAIN
_SCSI_TEST_OKAY:
	; return status disk present but changed
	ld 	a, (ix+WRKAREA.STATUS)
	set	3, a				; usb device mounted
	set	4, a				; ???
	set	5, a				; dsk changed
	ld	(ix+WRKAREA.STATUS), a

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
	CALL	MY_GWORK
	ld a,(ix+WRKAREA.STATUS)

	ld c, a
	ld a, b
	cp 0
	jr z,_DEV_INFO_BASIC
	cp 1
	jr z,_DEV_INFO_MANUFACTURER
	cp 2
	jr z,_DEV_INFO_DEVICE_NAME
	cp 3
	jr z,_DEV_INFO_SERIAL
	; else
	jr _DEV_INFO_NOT_INSERTED

_DEV_INFO_BASIC:
	ld (hl),1
	inc hl
	ld (hl),0
	xor a
	ret

_DEV_INFO_MANUFACTURER:
	ld de, hl
	ld bc, WRKAREA.SCSI_DEVICE_INFO.VENDORID
	call WRKAREAPTR
	ld hl,ix
	ld bc, 8
	ldir
	xor a
	ld (de),a
	ret

_DEV_INFO_DEVICE_NAME:
	ld de, hl
	ld bc, WRKAREA.SCSI_DEVICE_INFO.PRODUCTID
	call WRKAREAPTR
	ld hl,ix
	ld bc, 16
	ldir
	xor a
	ld (de),a
	ret

_DEV_INFO_SERIAL:
	ld de, hl
	ld bc, WRKAREA.SCSI_DEVICE_INFO.PRODUCTREV
	call WRKAREAPTR
	ld hl,ix
	ld bc, 4
	ldir
	xor a
	ld (de),a
	ret

_DEV_INFO_NOT_INSERTED:
	ld	a,1
	ret

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


DEV_WRT_USB:
	DI
	call	CAPS_FLASH
	call	SCSI_WRITE
	jr	c, _DEV_RW_ERR

_DEV_RW_NEXT_4:
	; CAPS OFF
	in	a, 0xaa
	set	6, a
	out	0xaa, a
	xor	a ; success
	ret

_DEV_RW_ERR
	call	CAPS_FLASH
	ld	a, _RNF
	ld	b, 0
	ret

DEV_READ_USB:
	DI
	call	CAPS_FLASH
	call	SCSI_READ
	jr	c, _DEV_RW_ERR
	jr	_DEV_RW_NEXT_4

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


CH376_NOT_FOUND_MSG:		DB	"CH376:           NOT PRESENT", 13, 10, 0
CH376_FOUND_MSG:		DB	"CH376:           PRESENT (VER ", 0
CH376_NEWLINE:			DB	")\r\n",0

USB_NOT_FOUND_MSG:		DB	"USB:             NOT PRESENT", 13, 10, 0
USB_FLASH_FOUND_MSG:		DB	"USB-STORAGE:     PRESENT", 13, 10, 0
USB_FLASH_NOT_FOUND_MSG:	DB	"USB-STORAGE:     NOT PRESENT", 13, 10, 0

    STRUCT _SCSI_READ_CAPACITY
BASE:			; Offset to the base of the data structure
OPERATION_CODE:		db 0x25
LUN:	            	db 0
RESERVED:		ds 8, 0
PAD:			ds 2, 0
    ENDS

SCSI_READ_CAPACITY          _SCSI_READ_CAPACITY