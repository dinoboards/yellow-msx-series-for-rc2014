

memmap_slot_jumps:

memmap_allocate_segment:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

memmap_free_segment:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET
