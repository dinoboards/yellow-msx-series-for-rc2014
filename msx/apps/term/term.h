/*
--
-- term.h
--   Simple TELNET client using UNAPI for MSX.
--   Revision 1.34
--
-- Requires SDCC and Fusion-C library to compile
-- Copyright (c) 2019-2020 Oduvaldo Pavan Junior ( ducasp@gmail.com )
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

#ifndef _TELNET_HEADER_INCLUDED
#define _TELNET_HEADER_INCLUDED

#include "msx_fusion.h"

/* must include stdbool before stdio, as stdio include msx.h which will define its own bool if none defined */
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines for TELNET negotiations
// Telnet Protocol Definitions
#define DO   0xfd
#define WONT 0xfc
#define WILL 0xfb
#define DONT 0xfe
#define IAC  0xff
#define SB   0xfa
#define SE   0xf0
#define IS   0
#define SEND 1
// Telnet 2 bytes commands
#define GA  0xf9
#define EL  0xf8
#define EC  0xf7
#define AYT 0xf6
#define AO  0xf5
#define IP  0xf4
#define BRK 0xf3
#define DM  0xf2
#define NOP 0xf1
// Telnet 3 bytes Commands
#define CMD_TRANSMIT_BINARY     0
#define CMD_ECHO                1
#define CMD_SUPPRESS_GO_AHEAD   3
#define CMD_TTYPE               24 // 0x18
#define CMD_WINDOW_SIZE         31 // 0x1f
#define CMD_TERMINAL_SPEED      32 // 0x20
#define CMD_REMOTE_FLOW_CONTROL 33 // 0x21
#define CMD_LINE_MODE           34 // 0x22
#define CMD_ENV_VARIABLES       36 // 0x24
#define CMD_ENCRYPTION          38 // 0x26

enum TelnetDataParserStates {
  TELNET_IDLE           = 0,
  TELNET_CMD_INPROGRESS = 1,
  TELNET_SUB_INPROGRESS = 2,
  TELNET_SUB_WAITEND    = 3,
  TELNET_ESC_INPROGRESS = 4
};

extern const unsigned char ucWindowSize[];
extern const unsigned char ucWindowSize0[];
extern const unsigned char ucWindowSize1[];
extern const unsigned char ucTTYPE2[];
extern const unsigned char ucTTYPE3[];

// Auxiliary strings

#include "system-state.h"

void         negotiate(unsigned char *ucBuf);
unsigned int IsValidInput(char **argv, int argc);
void         ParseTelnetData(void);
void         SendCursorPosition(unsigned int uiCursorPosition) __z88dk_fastcall;
#endif // _TELNET_HEADER_INCLUDED
