
	EXTERN	_usb_host_init
	EXTERN	_usb_dev_info
	EXTERN	_usb_lun_info
	EXTERN	_usb_dev_status

	ORG	$5800
usb_host_init:
	JP	_usb_host_init
usb_dev_info:
	JP	_usb_dev_info
usb_lun_info:
	JP	_usb_lun_info
usb_dev_status:
	JP	_usb_dev_status