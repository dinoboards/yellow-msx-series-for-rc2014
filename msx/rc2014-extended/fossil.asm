
; All the code here is relocated to page 3 location during rominit


@FOSSIL_DRV_START:

	MODULE	FOSSIL
	PHASE	0
	RELOCATE_START

	jp	getversion	; Version H.L (H,L packed BCD)
	jp	init		; initialises RS232
	jp	deinit		; deinitialises the RS232

	jp	set_baud	; H=Tx baud rate, L=Rx baud rate
				; 0  = 75	 1  = 300	  2  = 600
				; 3  = 1200	 4  = 2400	  5  = 4800
				; 6  = 9600	 7  = 19200	  8  = 38400
				; 9  = 57600	 11 = 115200

; if < 5, then 5
; if > 7, then 7


; Extra information :
;	In register H and L, the driver reports the actually selected rate.
;	This is done in case the hardware does not support the selected
;	rate. If the driver does not support the selected rate, it selects
;	the highest possible rate below the selected.

	jp	set_protocol	; H 0-1 data bits
				;	00 5 bits or less
				;	01 6 bits
				;	10 7 bits
				;	11 8 bits
				;   2-3 stop bits
				;	01 1 stopbit
				;	10 1.5 stopbits
				;	11 2 stopbits
				;   4-5 parity
				;	00 none
				;	01 even
				;	11 odd
				;   6-7 0
				; L = 0 (has to be set to zero, for future
				;	 extensions!)
	jp	channel 	; H	channel number (0-1) (NMS only)

	jp	rs_in		; read from RS232 buffer, A = character/byte
	jp	rs_out		; write to RS232, A = character/byte

	jp	rs_in_stat	; A=0 No data in buffer, A!=0 data in buffer
				; The F register is set according the result
	jp	rs_out_stat	; At this time every time=!0, but this is
				; reserved for future expansion of the driver
				; to sent data on interrupt. This function
				; is not needed to use, rs_in_stat is required
				; before rs_in is done.

	jp	dtr		; H=0 drop DTR, H=255 raise DTR
	jp	rts		; H=0 drop RTS, H=255 raise RTS

	jp	carrier 	; A=0 no carrier, A!= carrier detect

	jp	chars_in_buf	; Return : HL = characters in Rx buf.
	jp	size_of_buf	; Return : HL = size of buffer
	jp	flushbuf	; FLUSH THE RECEIVE BUFFER

	jp	fastint		; use &H0038 hook for speedup
				; H=0 Connect driver fast
				; H=1 Release fast hook

; This fastint needs some extra attention:
; The driver uses hook &HFD9A, but this is not very fast. Since this hook
; is called after some BIOS work, and after this hook, the BIOS continues
; to do some time waisting things.
; Therefore, this driver has the option to install itselve at the &H38 hook.
; This can be done if there is RAM available at &H0000-&H3FFF. This is the
; case in DOS(2).

	jp	hook38stat	; set status for 0038 hook
				; h  = 0, every interrupt is supported
				; h != 0, only RS232 interrupt (and VDP)

; This function is for use with fastint.
; When the "connect driver fast" has been issued, you can control how the
; interrupt handler of the driver behaves.
; If every interrupt is supported, there is slightly more processor time
; used after an interrupt from the RS232C interface.

	jp	chput_hook	; redirect CHPUT data to RS232
				; H=0 no redirection
				; H=1 redirect with echo
				; H=3 redirect without echo (faster)

; All print commands that would be issued using the BIOS calls, will be
; transmitted through the RS232C interface.

; With this function, you can redirect all incoming RS232C data to the
; keyboard buffer.
; This and the previous function are nice to let a terminal control your
; MSX computer.
; redirect RS232 to keyboard buffer
; H=0 release hook, H!=0 bend hook
	jp	keyb_hook

; Gives a pointer in HL that points to a
; driver info block. This block describes
; the current situation about the driver.
	jp	get_info


getversion:
	LD	HL, 0x0101
	RET

init:
	RST	$30
	DB	$8F				; slot 3-3
	DW	fossil_initialise
	EI
	RET

deinit:
	RST	$30
	DB	$8F				; slot 3-3
	DW	fossil_deinit
	EI
	RET

set_baud:
	RST	$30
	DB	$8F				; slot 3-3
	DW	fossil_set_baud
	EI
	RET

rs_in:
	push_page_1

	call	segment1_rs_in

	pop_page_1

	LD	A, C			; MOVE CHAR TO RETURN TO A
	OR	A
	RET

rs_out:
	EX	AF, AF'
	push_page_1
	EX	AF, AF'

	call	segment1_rs_out

	pop_page_1
	RET

rs_in_stat:
	LD	A, (RS_DATCNT)
	OR	A
	RET


chars_in_buf:
	LD	A, (RS_DATCNT)
	LD	L, A
	LD	H, 0

set_protocol:
channel:
rs_out_stat:
dtr:
carrier:
rts:
size_of_buf:
flushbuf:
fastint:
hook38stat:
chput_hook:
keyb_hook:
get_info:
	RET

@SIO_INT:
	DI				; INTERRUPTS WILL BE RE-ENABLED BY MSX BIOS

	; CHECK TO SEE IF SOMETHING IS ACTUALLY THERE
	XOR	A			; READ REGISTER 0
	OUT	(CMD_CH), A
	IN	A, (CMD_CH)
	AND	$01			; ISOLATE RECEIVE READY BIT
	JP	Z, RS_OLDINT		; NOTHING AVAILABLE ON CURRENT CHANNEL

	push_page_1

	call	segment1_sio_interrupt

	pop_page_1

	ret

	RELOCATE_END
	UNPHASE
@FOSSIL_DRV_LENGTH	EQU	$-FOSSIL_DRV_START

@FOSSILE_DRV_MAP:
	RELOCATE_TABLE
@FOSSILE_DRV_MAP_LENGTH	EQU	($-@FOSSILE_DRV_MAP)/2

	ENDMODULE
