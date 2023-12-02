/*
--
-- UnapiHelper.c
--   UNAPI Abstraction functions.
--   Revision 0.60
--
-- Requires SDCC and Fusion-C library to compile
-- Copyright (c) 2020 Andres Ortiz and Oduvaldo Pavan Junior ( ducasp@gmail.com )
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
#include "aofossilhelper.h"
#include "arguments.h"
// #include "fossil.h"
#include "msx2ansi.h"
#include "msx_fusion.h"
#include "print.h"
#include "system-state.h"
#include <extbio/serial.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char          chHelperString[128];
// unsigned char ucFossilUnsafeDataTXBuffer[128];
// const char    modem_atz[] = "ATZ\r\n";
// const char    modem_cmd[] = "+++";
// const char    modem_disc[] = "ATH\r\n";
// unsigned char cmdline[134];

uint8_t port_number = 1;

void Breath(void) { return; }

unsigned char InitializeTCPIP(void) {
  uint8_t buffer[64];

  serial_set_protocol(port_number, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);

  uint8_t result = serial_set_baudrate(port_number, baud_rate);
  sprintf(buffer, "Baud Rate: %ld (%02X)\r\n", baud_rate, result);
  AnsiPrint(buffer);

  serial_purge_buffers(port_number);
  return 1;
}

void CloseConnection(void) {}

// This routine retrieves as much as bytes as indicated in uiSize
// Note that uiSize=1024 in normal characters receiving mode (RcvMemorySize=1024 in term.h)
// Receives up to uiSize bytes
// Number of bytes retrieved from serial port are returned into uiSize
unsigned char RXData(unsigned char *ucBuffer, unsigned int *uiSize, unsigned char ucWaitAllDataReceived) {
  (void)ucWaitAllDataReceived;

  if (ucWaitAllDataReceived)
    serial_demand_read(port_number, ucBuffer, uiSize, 1000);
  else
    serial_read(port_number, ucBuffer, uiSize);

  return uiSize != 0;
}

// The same as TxUnsafeData but without page 3 buffer addressing
unsigned char TxData(const unsigned char *lpucData, unsigned int uiDataSize) {
  return serial_write(port_number, lpucData, uiDataSize);
}

// This routine sends only one byte
unsigned char TxByte(unsigned char uchByte) { return TxData(&uchByte, 1); }

unsigned char TxUnsafeData(const unsigned char *lpucData, unsigned int uiDataSize) { return TxData(lpucData, uiDataSize); }
