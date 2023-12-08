	include	"msx.inc"

	PUBLIC	_extbio
	PUBLIC	_extbio_next
	EXTERN	_tty_update

	SECTION	CODE


_extbio:
	EX	AF, AF'
	LD	A, D

	CP	EXTBIO_RC2014
	JR	Z, handle_extbio

	EX	AF, AF'

EXBIO_EXIT:
	DB	$C3	; JP opcode
_extbio_next:
	DW	0

handle_extbio:
	LD	A, E
	CP	EXTBIO_RC2014_TTY_FN
	JP	NZ, EXBIO_EXIT

EXTBIO_RC2014_TTY:
	LD	A, C
	CP	EXTBIO_RC2014_TTY_UPDATE_FN
	JP	Z, _tty_update


EXTBIO_UNKNOWN_SUB:
	LD	HL, -1	; UNKNOWN SUB FUNCTION
	RET

	PUBLIC	_timi
	PUBLIC	_timi_next

	EXTERN	_map_serial_to_keyboard
	EXTERN	_activate_stdin_port_number
	EXTERN	_activate_stdout_port_number
	EXTERN	__timi
_timi:
	; LD	A, (_activate_stdin_port_number)
	; OR	A
	CALL	__timi

	DB	$C3	; JP opcode
_timi_next:
	DW	0

;
; extern uint8_t serial_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size);
;
	PUBLIC	_serial_read_di:
_serial_read_di:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN ; RC2014 EXTENDED DRIVER/FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_READ_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN L
	POP	IX
	RET

;
; extern uint8_t serial_write_di(const uint8_t port_number, const uint8_t *const buf, const uint8_t size);
;
	PUBLIC	_serial_write_di:
_serial_write_di:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_WRITE_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET


	PUBLIC	_chput
	EXTERN	_original_hchpu
	EXTERN	_chput_to_serial
	EXTERN	_activate_crt

_chput:
	PUSH	AF
	LD	L, A
	LD	A, (_activate_stdout_port_number)
	OR	A
	CALL	NZ, _chput_to_serial

	POP	AF
_original_hchpu:
	DS	5


	SECTION	DATA

