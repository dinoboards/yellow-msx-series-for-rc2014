
	EXTERN	_usb_host_init
	EXTERN	_usb_dev_info
	EXTERN	_usb_lun_info
	EXTERN	_usb_dev_status
	EXTERN	_usb_dev_read
	EXTERN	_usb_dev_write
	EXTERN  _fn_enum
	EXTERN	_drv_direct4
	EXTERN	_drv_direct1
	EXTERN	_device_mapping
	EXTERN	_convert_and_print_disk_size
	EXTERN	_extbio_rc2014
	EXTERN	_drv_timi

	ORG	$5000
usb_host_init:
	JP	_usb_host_init
usb_dev_info:
	JP	_usb_dev_info
usb_lun_info:
	JP	_usb_lun_info
usb_dev_status:
	JP	_usb_dev_status
usb_dev_read:
	JP	_usb_dev_read
usb_dev_write:
	JP	_usb_dev_write
fn_enum:
	JP	_fn_enum
drv_direct4:
	JP	_drv_direct4
device_mapping:
	JP	_device_mapping
convert_and_print_disk_size:
	JP	_convert_and_print_disk_size
extbio_rc2014:
	JP	_extbio_rc2014
drv_direct1:
	JP	_drv_direct1
drv_timi:
	JP	_drv_timi
