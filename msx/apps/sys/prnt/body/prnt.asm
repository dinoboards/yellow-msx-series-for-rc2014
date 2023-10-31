	SECTION	CODE

	EXTERN	_drv_timi_printer
	EXTERN	_LOWLIM
	EXTERN	__printer_config
	EXTERN	_usb_data_out_transfer


	PUBLIC	_timi
	PUBLIC	_timi_next

_timi:
; ;sys/prnt/body/prnt.c:10: if (!(LOWLIM & 0x80))

; 	ld	a, (_LOWLIM)
; 	rlca
; ;sys/prnt/body/prnt.c:11: return;
; 	jr	NC,l_drv_timi_printer_00108

; ;sys/prnt/body/prnt.c:13: if (_printer_config.buffer_length == 0) {
; 	ld	a, (__printer_config + 6)
; ;sys/prnt/body/prnt.c:13: if (_printer_config.buffer_length == 0) {
; 	or	a
; 	jr	NZ, l_drv_timi_printer_00104

; ;sys/prnt/body/prnt.c:14: _printer_config.buffer_wait = 0;
; 	ld	(__printer_config + 7), a

; ;sys/prnt/body/prnt.c:15: LOWLIM &= 0x7F;
; 	ld	a, (_LOWLIM)
; 	and	a, 0x7f
; 	ld	(_LOWLIM), a

; ;sys/prnt/body/prnt.c:16: return;
; 	jr	l_drv_timi_printer_00108


; l_drv_timi_printer_00104:
; ;sys/prnt/body/prnt.c:19: if (_printer_config.buffer_length != PRINTER_BUFFER_SIZE && _printer_config.buffer_wait != 5) {
; 	ld	a, (__printer_config + 6)
; 	sub	0x40
; 	jr	Z, l_drv_timi_printer_00106

; 	ld	a, (__printer_config + 7)
; 	cp	0x05
; 	jr	Z, l_drv_timi_printer_00106

; ;sys/prnt/body/prnt.c:20: _printer_config.buffer_wait++;
; 	inc	a
; 	ld	(__printer_config + 7), a

; ;sys/prnt/body/prnt.c:22: return;
; 	jr	l_drv_timi_printer_00108


; l_drv_timi_printer_00106:
; ;sys/prnt/body/prnt.c:26: &_printer_config.endpoints[0]);
; ;sys/prnt/body/prnt.c:25: usb_data_out_transfer(_printer_config.buffer, _printer_config.buffer_length, _printer_config.address,
; 	ld	a, (__printer_config)
; 	rlca
; 	rlca
; 	rlca
; 	rlca
; 	and	a, 0x0f
; 	ld	c, a
; 	ld	a, (__printer_config + 6)
; 	ld	e, a
; 	ld	d, 0x00
; 	ld	hl, __printer_config + 3
; 	push	hl
; 	ld	a, c
; 	push	af
; 	inc	sp
; 	push	de
; 	ld	hl, __printer_config + 8
; 	push	hl
; 	call	_usb_data_out_transfer
; 	pop	af
; 	pop	af
; 	pop	af
; 	inc	sp

; ;sys/prnt/body/prnt.c:27: _printer_config.buffer_length = 0;

; 	xor	a
; 	ld	(__printer_config + 6), a


; ;sys/prnt/body/prnt.c:28: _printer_config.buffer_wait   = 1;
; 	inc	a
; 	ld	(__printer_config + 7), a

; ;sys/prnt/body/prnt.c:29: LOWLIM &= 0x7F;
; 	ld	a, (_LOWLIM)
; 	and	a, 0x7f
; 	ld	(_LOWLIM), a

; l_drv_timi_printer_00108:

	CALL	_drv_timi_printer
	DB	$C3	; JP opcode
_timi_next:
	DW	0
