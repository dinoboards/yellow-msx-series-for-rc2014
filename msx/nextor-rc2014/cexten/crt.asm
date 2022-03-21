
	EXTERN	_usb_host_init
	EXTERN	_usb_dev_info
	ORG	$5800
usb_host_init:
	JP	_usb_host_init
usb_dev_info:
	JP	_usb_dev_info
