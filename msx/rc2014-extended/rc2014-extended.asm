
	ORG	08000H

	include "msx.inc"
	INCLUDE	"sio.inc"

; +0	ID		Put these first two bytes at 041H and 042H ("AB") to indicate that it is an additional ROM.
; +2	INIT		Address of the routine to call to initialize a work area or I/O ports, or run a game, etc.
; +4	STATEMENT	Runtime address of a program whose purpose is to add instructions to the MSX-Basic using CALL.
; +6	DEVICE		Execution address of a program used to control a device built into the cartridge. For example, a disk interface.
; +8	TEXT		Pointer of the tokenizen Basic program contained in ROM.
; +10	Reserved	6 bytes reserved for future updates.

ROM_HEADER:
	DB	"AB"
	DW	ROM_INIT
	DW	0
	DW	0
	DW	0
	DS	6

	include	"segment1.inc"
	INCLUDE "rominit.asm"
	INCLUDE	"fossil.asm"
	INCLUDE "alloc.asm"
	INCLUDE "sio.asm"
	INCLUDE	"utils.asm"
	INCLUDE "rs232.asm"
	include "probing.asm"
	include	"extended-bios.asm"
	include "segment1.asm"

	DS	$C000 - $
