	include	"msx.inc"

;
; extern uint32_t extbio_usb_get_device_descriptor(const uint8_t device_index, const device_descriptor* _device_descriptor) __sdcccall(1) {
;
	PUBLIC	_extbio_usb_get_device_descriptor
_extbio_usb_get_device_descriptor:
	PUSH	IX
	LD	D, 214		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_GET_DESCRIPTOR_FN		; FUNCTION CODE
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET
