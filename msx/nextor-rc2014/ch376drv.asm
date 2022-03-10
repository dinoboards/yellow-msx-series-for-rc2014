DRV_INIT_CH376:
	call	USBHOST_INIT

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
; 	; start communicating with SCSI device
; 	call SCSI_MAX_LUNS
; 	ret c
; 	call SCSI_INIT
; 	call SCSI_INQUIRY
; 	jr nc, _INQUIRY_OKAY
; 	ld hl, TXT_INQUIRY_NOK
; 	call PRINT
; 	ret
; _INQUIRY_OKAY:
; 	push ix
; 	;
; 	ld hl, TXT_INQUIRY_OK
; 	call PRINT
; 	ld bc, WRKAREA.SCSI_DEVICE_INFO.VENDORID
; 	call WRKAREAPTR
; 	ld hl, ix
; 	call PRINT
; 	ld hl, TXT_INQUIRY_OK
; 	call PRINT
; 	ld bc, WRKAREA.SCSI_DEVICE_INFO.PRODUCTID
; 	call WRKAREAPTR
; 	ld hl, ix
; 	call PRINT
; 	;
; 	pop ix
; 	ld a,(ix+WRKAREA.STATUS)
;     set 2,a
;     ld (ix+WRKAREA.STATUS),a

; 	ld hl, TXT_TEST_START
; 	call PRINT
; _SCSI_TEST_AGAIN:
; 	call SCSI_TEST
; 	jr nc, _SCSI_TEST_OKAY
; 	call SCSI_REQUEST_SENSE
; 	jr _SCSI_TEST_AGAIN
; _SCSI_TEST_OKAY:
; 	ld hl, TXT_TEST_OK
; 	call PRINT
; 	; return status disk present but changed
; 	ld a,(ix+WRKAREA.STATUS)
;     set 3,a
; 	set 4,a
; 	set 5,a
;     ld (ix+WRKAREA.STATUS),a
; 	ret

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
	; #0
	ld (hl),0 ; block device
	; #1
	inc hl
	ld (hl),00h
	inc hl
	ld (hl),02h ; 512 byte sector
	; #3

    inc hl

    push hl
    push ix
    ld a, (ix+WRKAREA.STORAGE_DEVICE_INFO.DEVICE_ADDRESS)
    ld bc, WRKAREA.SCSI_DEVICE_INFO.BUFFER
    add ix, bc
    ld iy, ix
    
    ld b, 0
    ld c, _SCSI_READ_CAPACITY
    ld de, 0x8
    ld hl, SCSI_READ_CAPACITY
    push iy
    or  a
    call DO_SCSI_CMD
    pop iy
    pop ix
    pop hl


    ld a, (iy+3)
    ld  (hl), a
    inc hl

    ld a, (iy+2)
    ld  (hl), a
    inc hl

    ld a, (iy+1)
    ld  (hl), a
    inc hl

    ld a, (iy+0)
    ld  (hl), a
    inc hl

	; #7
	; inc hl
	ld (hl),00000001b ; removable + non-read only + no floppy
	; #8
	inc hl
	ld (hl), 0
	inc hl
	ld (hl), 0 ; cylinders/tracks
	; #10
	inc hl
	ld (hl), 0 ; heads
	; #11
	inc hl
	ld (hl), 0 ; sectors per track

	ld a, 0
	ret


CH376_NOT_FOUND_MSG:		DB	"CH376:           NOT PRESENT", 13, 10, 0
CH376_FOUND_MSG:		DB	"CH376:           PRESENT (VER ", 0
CH376_NEWLINE:			DB	")\r\n",0

USB_NOT_FOUND_MSG:		DB	"USB:             NOT PRESENT", 13, 10, 0
USB_FLASH_FOUND_MSG:		DB	"USB-STORAGE:     PRESENT", 13, 10, 0
USB_FLASH_NOT_FOUND_MSG:	DB	"USB-STORAGE:     NOT PRESENT", 13, 10, 0
