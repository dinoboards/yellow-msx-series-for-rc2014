

	include "msxdos.inc"

	EXTERN	CDRVR_REGS_F
	EXTERN	msxdosDevRw

	SECTION	CODE


; extern uint16_t msxdosDevRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);
	PUBLIC	_msxdosDevRead
_msxdosDevRead:
	XOR	A			; SET FLAG FOR READ ACCESS
	LD	(CDRVR_REGS_F), A
	JR	msxdosDevRw
