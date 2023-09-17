#include "class-hub.h"
#include "work-area.h"

const setup_packet cmd_get_hub_descriptor = {RT_DEVICE_TO_HOST | RT_CLASS | RT_DEVICE, 6, {0, 0x29}, {0, 0}, 8};

usb_error hub_get_descriptor(hub_descriptor *const hub_description) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  return hw_control_transfer(&cmd_get_hub_descriptor, hub_description, work_area->hub_config.address,
                             work_area->hub_config.max_packet_size);
}
