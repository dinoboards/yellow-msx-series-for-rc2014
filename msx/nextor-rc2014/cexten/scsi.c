#include "scsi.h"

_scsi_command_block_wrapper scsi_command_block_wrapper = {{0x55, 0x53, 0x42, 0x43}, {0, 0, 0, 0}, 0, 0, 0, 0};

_scsi_packet_inquiry scsi_packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};

_scsi_packet_test scsi_packet_test = {0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}};

_scsi_packet_request_sense scsi_packet_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

_scsi_read_capacity scsi_read_capacity = {0x25, 0, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0}};

_scsi_packet_read scsi_packet_read = {0x28, 0, {0, 0, 0, 0}, 0, {0, 0}, 0, {0, 0}};