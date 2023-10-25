
	IF $4B00-$ < 0
		FAIL "Base driver overflowing into USB driver"
	ENDIF
	DS	$4B00-$, $FF
DRVEND:
