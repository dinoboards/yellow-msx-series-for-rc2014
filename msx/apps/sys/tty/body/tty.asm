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
	EXTERN	_activated_port_number

_timi:
	LD	A, (_activated_port_number)
	OR	A
	CALL	NZ, _map_serial_to_keyboard

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

	SECTION	DATA

