
	IF $4C00-$ < 0
		FAIL "Base driver overflowing into USB driver"
	ENDIF
	DS	$4C00-$, $FF
DRVEND:
