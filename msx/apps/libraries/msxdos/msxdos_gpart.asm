

	include "msxdos.inc"

	SECTION	BSS
	PUBLIC	_workingMsxDosBuff
	PUBLIC	GPARTINFO_WRK, CDRVR_REGS, CDRVR_REGS_A, CDRVR_REGS_B, CDRVR_REGS_C, CDRVR_REGS_L, CDRVR_REGS_H, CDRVR_REGS_BC, CDRVR_REGS_DE, CDRVR_REGS_HL, CDRVR_REGS_F

_workingMsxDosBuff:
GPARTINFO_WRK:
CDRVR_REGS:
CDRVR_REGS_F:	DB	0
CDRVR_REGS_A:	DB	0
CDRVR_REGS_BC:
CDRVR_REGS_C:	DB	0
CDRVR_REGS_B:	DB	0
CDRVR_REGS_DE:
CDRVR_REGS_E:	DB	0
CDRVR_REGS_D:	DB	0
CDRVR_REGS_HL:
CDRVR_REGS_L:	DB	0
CDRVR_REGS_H:	DB	0