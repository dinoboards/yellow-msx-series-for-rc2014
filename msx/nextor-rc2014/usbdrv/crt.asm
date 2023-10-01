
	EXTERN	_usb_host_init
	EXTERN	_usb_dev_info
	EXTERN	_usb_lun_info
	EXTERN	_usb_dev_status
	EXTERN	_usb_dev_read
	EXTERN	_usb_dev_write
	EXTERN  _fn_enum
	EXTERN	_get_number_of_usb_drives
	EXTERN	_drv_direct4
	EXTERN	_device_mapping
	EXTERN _convert_and_print_disk_size

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
get_number_of_usb_drives:
	JP	_get_number_of_usb_drives
drv_direct4:
	JP	_drv_direct4
device_mapping:
	JP	_device_mapping
convert_and_print_disk_size:
	JP	_convert_and_print_disk_size
