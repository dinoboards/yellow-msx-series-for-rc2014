
	IF $4D00-$ < 0
		FAIL "Base driver overflowing into USB driver"
	ENDIF
	DS	$4D00-$, $FF
DRVEND:
