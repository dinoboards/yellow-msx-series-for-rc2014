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

; marshal to c code:
; uint8_t usb_dev_info(const uint8_t device_index, const dev_info_request request_info, uint8_t *const buffer) {

DEV_INFO_USB:
	push	hl
	sub	DEV_MAP_USB1-1 		; change to range of 1-4
	ld	c, a
	push	bc
	call	_usb_dev_info
	pop	af
	pop	af
	ld	a, l
	RET

; marshal to c code:
; uint8_t usb_lun_info(const uint8_t device_index, const uint8_t lun, nextor_lun_info *const info) {
;
LUN_INFO_USB:
	push	hl
	sub	DEV_MAP_USB1-1 		; change to range of 1-4
	ld	c, a
	push	bc
	call	_usb_lun_info
	pop	af
	pop	af
	ld	a, l
	RET

; marshal to c code:
; uint8_t usb_dev_status(const uint8_t device_index, const uint8_t lun) {

DEV_STATUS_USB:
	sub	DEV_MAP_USB1-1 		; change to range of 1-4
	ld	c, a
	push	bc
	call	_usb_dev_status
	pop	bc
	ld	a, l
	ret

; marshal to c code
; uint8_t usb_dev_write(const uint8_t  device_index,
;                       const uint8_t  lun,
;                       uint8_t        number_sectors_to_write,
;                       uint32_t       sector_number,
;                       uint8_t *      buffer,
;                       uint8_t *const number_of_sectors_written)

DEV_WRT_USB:
	push	ix
	dec	sp

	sub	DEV_MAP_USB1-1 		; change to range of 1-4
	ex	af, af'

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
	ex	af, af'			; restore usb dev number
	push	af
	inc	sp

	call	_usb_dev_write

	pop	af			; restore stack
	pop	af
	pop	af
	pop	af
	pop	af
	inc	sp

	ld	a, l			; return values
	ld	b, (ix-1)

	inc	sp			; restore stack
	pop	ix
	ret

; marshal to c code:
; uint8_t usb_dev_read(const uint8_t  device_index,
;                      const uint8_t  lun,
;                      uint8_t        number_sectors_to_read,
;                      uint32_t       sector_number,
;                      uint8_t *      buffer,
;                      uint8_t *const number_of_sectors_read) {


DEV_READ_USB:
	push	ix
	dec	sp

	sub	DEV_MAP_USB1-1 		; change to range of 1-4
	ex	af, af'

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
	ex	af, af'			; restore usb dev number
	push	af
	inc	sp

	call	_usb_dev_read
	pop	af			; restore stack
	pop	af
	pop	af
	pop	af
	pop	af
	inc	sp

	ld	a, l			; return values
	ld	b, (ix-1)

	inc	sp			; restore stack
	pop	ix
	ret

