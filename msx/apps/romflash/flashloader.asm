
	PUBLIC	_flashLoader

; extern void flashLoader(const uint8_t segment) __z88dk_fastcall;

_flashLoader:
	EXX				; SAVE HL

	; CLEAR PAGE 3 RAM BANK 
	; USING FAST SP TECHNIQUE (https://www.cpcwiki.eu/index.php/Programming:Filling_memory_with_a_byte)
	LD	SP, $FFFE		; MUST NOT WRITE TO $FFFF - AS THIS IS THE SUB SLOT SELECTOR
	LD	HL, 0    		; 16 BIT VALUE TO WRITE TO RAM

	; SET UP A FAST 16-BIT LOOP COUNTER FOR 16K less 16 bytes
	ld	b, $f0
	ld	de, $20EF

	; ERASE PAGE 3 TO 0
PUSHLOOP:
	PUSH	HL
	DJNZ	PUSHLOOP
	DEC	D
	JR	NZ, PUSHLOOP

	ld b,b
	jr $+2
	
	LD	DE, $C100
	LD	HL, ROM_FLASH_WRITER
	LD	BC, ROM_FLASH_WRITER_LENGTH
	LDIR
	JP	$C100

ROM_FLASH_WRITER:
	include	"../bin/romflashwriter/write-extended.inc"
ROM_FLASH_WRITER_LENGTH	EQU	$-ROM_FLASH_WRITER
