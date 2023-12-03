
	include "msx.inc"
	include "../sio.inc"

	SECTION	CODE
	PUBLIC	_sio2_chip_init

_sio2_chip_init:
	LD	C, RC_SIOA_CMD
	SIO_CHIP_INIT_C SIO_RTSOFF

	LD	C, RC_SIOB_CMD
	SIO_CHIP_INIT_C SIO_RTSOFF

	RET
