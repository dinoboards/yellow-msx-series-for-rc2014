
	PUBLIC	_flashLoader

; extern void flashLoader();

_flashLoader:
	LD	DE, 0C000H
	LD	HL, ROM_FLASH_WRITER
	LD	BC, ROM_FLASH_WRITER_LENGTH
	LDIR
	JP	0C000H

ROM_FLASH_WRITER:
	include	"../bin/romflashwriter/romflashwriter.inc"
ROM_FLASH_WRITER_LENGTH	EQU	$-ROM_FLASH_WRITER
