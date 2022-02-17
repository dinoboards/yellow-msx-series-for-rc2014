
	PUBLIC	_flash_loader_extended
_flash_loader_extended:
	; CLEAR PAGE 3 RAM BANK 
	; USING FAST SP TECHNIQUE (https://www.cpcwiki.eu/index.php/Programming:Filling_memory_with_a_byte)
	LD	SP, $FFFE		; MUST NOT WRITE TO $FFFF - AS THIS IS THE SUB SLOT SELECTOR
	LD	HL, 0    		; 16 BIT VALUE TO WRITE TO RAM

	; SET UP A FAST 16-BIT LOOP COUNTER FOR 16K less 16 bytes
	ld	b, $f0
	ld	de, $20EF

CLRLP1:
	PUSH	HL
	DJNZ	CLRLP1
	DEC	D
	JR	NZ, CLRLP1

	LD	DE, $C100
	LD	HL, WRITE_EXTENDED
	LD	BC, WRITE_EXTENDED_LENGTH
	LDIR
	JP	$C100

WRITE_EXTENDED:
	include	"../bin/romflashwriter/write-extended.inc"
WRITE_EXTENDED_LENGTH	EQU	$-WRITE_EXTENDED

	PUBLIC	_flash_loader_driver
_flash_loader_driver:
	ld b,b
	jr $+2
	; CLEAR PAGE 3 RAM BANK 
	; USING FAST SP TECHNIQUE (https://www.cpcwiki.eu/index.php/Programming:Filling_memory_with_a_byte)
	LD	SP, $FFFE		; MUST NOT WRITE TO $FFFF - AS THIS IS THE SUB SLOT SELECTOR
	LD	HL, 0    		; 16 BIT VALUE TO WRITE TO RAM

	; SET UP A FAST 16-BIT LOOP COUNTER FOR 16K less 16 bytes
	ld	b, $f0
	ld	de, $20EF

	; ERASE PAGE 3 TO 0
CLRLP2:
	PUSH	HL
	DJNZ	CLRLP2
	DEC	D
	JR	NZ, CLRLP2

	LD	DE, $C100
	LD	HL, WRITE_DRIVER
	LD	BC, WRITE_DRIVER_LENGTH
	LDIR
	JP	$C100

WRITE_DRIVER:
	include	"../bin/romflashwriter/write-driver.inc"
WRITE_DRIVER_LENGTH	EQU	$-WRITE_DRIVER

