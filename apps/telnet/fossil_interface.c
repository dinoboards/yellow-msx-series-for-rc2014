/*
--
-- fossil_interface.h
--   Erik Maas Fossil Driver interface for Fusion-C.
--   Revision 1.0
--
-- Requires SDCC and Fusion-C library to compile
-- Copyright (c) 2020 - Andres Ortiz

-- All rights reserved.
--
-- Redistribution and use of this source code or any derivative works, are
-- permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice,
--    this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Redistributions may not be sold, nor may they be used in a commercial
--    product or activity without specific prior written permission.
-- 4. Source code of derivative works MUST be published to the public.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
-- TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
-- PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
-- CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
-- EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
-- PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
-- OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
-- WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
-- OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
-- ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--
*/

#include "fossil_interface.h"
#include "msx_fusion.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BCD to decimal conversion for BCD packed numbers
inline uint8_t bcd_to_decimal(unsigned char x) { return x - 6 * (x >> 4); }

// Show info contained in the info block returned by Fossil_get_info
void show_info(void) {
  const fossile_info *info = fossil_get_info();
  printf("Driver Version: %d.%d\r\n", bcd_to_decimal(info->version_number >> 8), bcd_to_decimal(info->version_number & 255));
  printf("RX Speed: %d\r\n", info->rx_speed_index);
  printf("TX Speed: %d\r\n", info->tx_speed_index);
  printf("Protocol: %d\r\n", info->protocol);
  printf("ChPut_hook: %d\r\n", info->chput_hook_status);
  printf("Keyboard_hook: %d\r\n", info->keyb_hook_status);
  printf("RTS Status: %d\r\n", info->rts_status);
  printf("DTR Status: %d\r\n", info->dtr_status);
  printf("Current Channel: %d\r\n", info->channel);
  printf("Hardware info: %d\r\n", info->hardware);
}
