	PUBLIC _install_extbio_hook
	EXTERN	_sys_segment_head
	EXTERN	_call_mapi
	EXTERN	_allocated_segment
	include "msx.inc"

defc _original_extbio_hook = 0x4000 + 64*3;
defc _original_timi_hook = _original_extbio_hook+5;
defc _original_keyi_hook = _original_timi_hook+5;

_install_extbio_hook:
	LD	A, (RS_HOKVLD)
	BIT	0, A
	JR	NZ, INST_HOOK

	; THERE IS NO EXTBIO INSTALL - SO WE HAVE TO INITIALISE
	LD	A, 1
	LD	(RS_HOKVLD), A
	LD	A, $C9
	LD	(H_BEXT), A		; SET OPERATION CODE OF 'RET'

INST_HOOK:
	LD	A, 0xC3				; JUMP OP CODE
	LD	(_sys_segment_head), A
	LD	HL, _original_extbio_hook	; original_extbio_hook
	LD	(_sys_segment_head+1), HL

	LD	DE, _original_extbio_hook	; COPY EXTBIO HOOK FUNCTION FROM H_BEXT TO RS_MEXBIH
	LD	HL, H_BEXT
	LD	BC, 5
	LDIR


	DI
	; SET A RAM HELPER CALLSEG TO
	; MAKE EXTBIO JUMP TO 0x4000 FOR
	; OUR ALLOCATED SEGMENT
	LD	A, $CD			; CALL OP CODE
	LD	(H_BEXT), A

	ld	hl, (_call_mapi)
	LD	(H_BEXT+1), HL		; SET SLOT ADDRESS

	XOR	A
	LD	(H_BEXT+3), A		; MAIN MEMORY MAPPER SLOT @4000

	LD	A, (_allocated_segment)	;
	LD	(H_BEXT+4), A		;
	EI
	RET

	PUBLIC	_install_timi_hook

_install_timi_hook:
	LD	A, 0xC3				; JUMP OP CODE
	LD	(_sys_segment_head + 3), A
	LD	HL, _original_timi_hook
	LD	(_sys_segment_head+4), HL

	LD	DE, _original_timi_hook
	LD	HL, H_TIMI
	LD	BC, 5
	LDIR

	DI
	; SET A RAM HELPER CALLSEG TO
	; MAKE EXTBIO JUMP TO 0x4003 FOR
	; OUR ALLOCATED SEGMENT
	LD	A, $CD			; CALL OP CODE
	LD	(H_TIMI), A

	ld	hl, (_call_mapi)
	LD	(H_TIMI+1), HL		; SET SLOT ADDRESS

	LD	A, 1			; INDEX 1 FOR 0x4003
	LD	(H_TIMI+3), A		; MAIN MEMORY MAPPER SLOT @4000

	LD	A, (_allocated_segment)	;
	LD	(H_TIMI+4), A		;
	EI
	RET

	PUBLIC	_install_keyi_hook

_install_keyi_hook:
	LD	A, 0xC3				; JUMP OP CODE
	LD	(_sys_segment_head + 6), A
	LD	HL, _original_keyi_hook
	LD	(_sys_segment_head+7), HL

	LD	DE, _original_keyi_hook
	LD	HL, H_KEYI
	LD	BC, 5
	LDIR

	DI
	; SET A RAM HELPER CALLSEG TO
	; MAKE EXTBIO JUMP TO 0x4003 FOR
	; OUR ALLOCATED SEGMENT
	LD	A, $CD			; CALL OP CODE
	LD	(H_KEYI), A

	ld	hl, (_call_mapi)
	LD	(H_KEYI+1), HL		; SET SLOT ADDRESS

	LD	A, 2			; INDEX 1 FOR 0x4006
	LD	(H_KEYI+3), A		; MAIN MEMORY MAPPER SLOT @4000

	LD	A, (_allocated_segment)	;
	LD	(H_KEYI+4), A		;
	EI
	RET
