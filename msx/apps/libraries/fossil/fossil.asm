
	include	"msx.inc"

	SECTION	CODE
; extern void fossil_link() __z88dk_fastcall
;
	PUBLIC	_fossil_link

_fossil_link:
	LD	L, 0
	LD	A, (FSMARK)
	CP	'S'
	RET	NZ
	LD	A, (FSMARK+1)
	CP	'R'
	RET	NZ

	LD	HL, (FSTABL)
	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(fossil_get_version_prt+1), de
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(fossil_init_ptr+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_deinit+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_set_baud+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_set_protocol+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_channel+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_rs_in+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(rs_out_ptr+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_rs_in_stat+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_rs_out_stat+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_dtr+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_rts+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_carrier+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_chars_in_buf+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_size_of_buf+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_flushbuf+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_fastint+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_hook38stat+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_chput_hook+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_keyb_hook+1), DE
	INC	HL

	INC	HL
	LD	E, (HL)
	INC	HL
	LD	D, (HL)
	LD	(_fossil_get_info+1), DE

	LD	HL, 1
	RET

	PUBLIC	_fossil_get_version
	PUBLIC	_fossil_init
	PUBLIC	_fossil_deinit
	PUBLIC	_fossil_set_baud
	PUBLIC	_fossil_set_protocol
	PUBLIC	_fossil_channel
	PUBLIC	_fossil_rs_in
	PUBLIC	_fossil_rs_out
	PUBLIC	_fossil_rs_in_stat
	PUBLIC	_fossil_rs_out_stat
	PUBLIC	_fossil_dtr
	PUBLIC	_fossil_rts
	PUBLIC	_fossil_carrier
	PUBLIC	_fossil_chars_in_buf
	PUBLIC	_fossil_size_of_buf
	PUBLIC	_fossil_flushbuf
	PUBLIC	_fossil_fastint
	PUBLIC	_fossil_hook38stat
	PUBLIC	_fossil_chput_hook
	PUBLIC	_fossil_keyb_hook
	PUBLIC	_fossil_get_info

_fossil_get_version:
fossil_get_version_prt:
	JP	0

_fossil_init:
fossil_init_ptr:
	JP	0

_fossil_deinit:
	JP	0

_fossil_set_baud:
	JP	0

_fossil_set_protocol:
	JP	0

_fossil_channel:
	JP	0

_fossil_rs_in:
	CALL	0
	LD	L, A
	LD	H, 0
	RET

_fossil_rs_out:
	LD	A, L
rs_out_ptr:
	JP	0

_fossil_rs_in_stat:
	CALL	0
	LD	L, A
	RET

_fossil_rs_out_stat:
	JP	0

_fossil_dtr:
	JP	0

_fossil_rts:
	JP	0

_fossil_carrier:
	JP	0

_fossil_chars_in_buf:
	JP	0

_fossil_size_of_buf:
	JP	0

_fossil_flushbuf:
	JP	0

_fossil_fastint:
	JP	0

_fossil_hook38stat:
	JP	0

_fossil_chput_hook:
	JP	0

_fossil_keyb_hook:
	JP	0

_fossil_get_info:
	JP	0
