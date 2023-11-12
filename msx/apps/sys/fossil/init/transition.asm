
	PUBLIC	_transition
defc _transition = 0xC000

	PUBLIC	_transition_start
	PUBLIC	_transition_end


	SECTION	CODE
_transition_start:
	PHASE 0xC000

	LD	C, E
	LD	B, D
	LD	DE, 0x100
	LDIR
	JP	0x100

	DEPHASE

_transition_end:

	PUBLIC	_temp_storage
	SECTION	BSS_END
_temp_storage:
