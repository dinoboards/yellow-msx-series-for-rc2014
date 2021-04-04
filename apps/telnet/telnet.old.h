/*
--
-- telnet.h
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

#include "asm.h"
#include "msx_fusion.h"
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

enum TelnetDataParserStates { TELNET_IDLE = 0, TELNET_CMD_INPROGRESS = 1, TELNET_SUB_INPROGRESS = 2, TELNET_SUB_WAITEND = 3, TELNET_ESC_INPROGRESS = 4 };

// Those won't change, so we won't waste memory and use global constants
const unsigned char ucWindowSize[] = {IAC, WILL, CMD_WINDOW_SIZE, IAC, SB, CMD_WINDOW_SIZE, 0, 80, 0, 24, IAC, SE};  // our terminal is 80x24
const unsigned char ucWindowSize0[] = {IAC, WILL, CMD_WINDOW_SIZE, IAC, SB, CMD_WINDOW_SIZE, 0, 40, 0, 24, IAC, SE}; // our terminal is 40x24
const unsigned char ucWindowSize1[] = {IAC, WILL, CMD_WINDOW_SIZE, IAC, SB, CMD_WINDOW_SIZE, 0, 80, 0, 25, IAC, SE}; // our terminal is 80x25
const unsigned char ucTTYPE2[] = {IAC, SB, CMD_TTYPE, IS, 'A', 'N', 'S', 'I', IAC, SE};                              // Terminal ANSI
const unsigned char ucTTYPE3[] = {IAC, SB, CMD_TTYPE, IS, 'V', 'T', '5', '2', IAC, SE};                              // Terminal UNKNOWN

// Auxiliary strings
const unsigned char ucCrLf[3] = "\r\n"; // auxiliary

// Instructions
const char ucUsage[] = "Usage: telnet <server[:port]> [a] [r] [o] [c]\r\n\r\n"
                       "<server[:port]>: 192.168.0.1:34 or bbs.hispamsx.org\r\n\r\n"
                       "a: turns off automatic download detection (some BBSs can't be used with it)\r\n"
                       "o: turns off ANSI rendering and use MSX-DOS text rendering\r\n"
                       "c: turns off custom CP437 font on MSX1\r\n"
                       "r: if file transfer fails try using this, some BBSs misbehave on file transfers\r\n\r\n";

const char ucSWInfo[] = "> MSX BaDCaT ANSI Terminal v1.34 <\r\n(c) 2020 Oduvaldo Pavan Junior - ducasp@gmail.com\r\nContributor: Andres Ortiz\r\n\r\n";
const char ucSWInfoANSI[] = "\x1b[37m> MSX BaDCaT ANSI Terminal v1.34 <\r\n\x1b[31m](c) 2020 Oduvaldo Pavan Junior - ducasp@gmail.com\r\nContributor: Andres Ortiz\r\n\r\n";

const char ucCursor_Up[] = "\x1b[A";
const char ucCursor_Down[] = "\x1b[B";
const char ucCursor_Forward[] = "\x1b[C";
const char ucCursor_Backward[] = "\x1b[D";
const char ucHispaBD[] = {
    0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xDD, 0x1B, 0x5B, 0x33, 0x37, 0x6D, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xDE, 0x1B, 0x5B, 0x31, 0x3B, 0x33,
    0x33, 0x3B, 0x34, 0x31, 0x6D, 0xB0, 0x1B, 0x5B, 0x30, 0x3B, 0x33, 0x31, 0x6D, 0xDC, 0x1B, 0x5B, 0x33, 0x37, 0x6D, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xDC, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x33, 0x3B,
    0x34, 0x31, 0x6D, 0xB1, 0xB1, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xB1, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xDC, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x33, 0x3B, 0x34, 0x31, 0x6D, 0xB0, 0xB2,
    0xB2, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xB2, 0x1B, 0x5B, 0x30, 0x3B, 0x33, 0x31, 0x6D, 0xDD, 0x1B, 0x5B, 0x33, 0x37, 0x6D, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xDE, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x33, 0x3B, 0x34,
    0x31, 0x6D, 0xB0, 0xB2, 0x1B, 0x5B, 0x34, 0x30, 0x6D, 0xDF, 0x1B, 0x5B, 0x33, 0x31, 0x3B, 0x34, 0x31, 0x6D, 0xB2, 0xB0, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x33, 0x31, 0x6D, 0xDB, 0x1B, 0x5B, 0x31,
    0x3B, 0x33, 0x33, 0x6D, 0xDD, 0x1B, 0x5B, 0x33, 0x31, 0x3B, 0x34, 0x31, 0x6D, 0xB0, 0x1B, 0x5B, 0x30, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x33, 0x37, 0x6D, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33,
    0x30, 0x6D, 0xDE, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDC, 0x20, 0x20, 0xDC, 0x20, 0xDC, 0x20, 0xDC, 0xDC, 0xDC, 0x20, 0xDC, 0xDC, 0xDC, 0xDC, 0x20, 0xDC, 0xDC, 0xDC, 0xDC, 0x20, 0x1B, 0x5B,
    0x33, 0x34, 0x6D, 0xDC, 0xDC, 0x20, 0xDC, 0xDC, 0x20, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0x20, 0x20, 0x20, 0xDC, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x33, 0x6D, 0xDB, 0xDC, 0xDC, 0xDB, 0x20, 0xDB, 0x20, 0xDC,
    0xDC, 0xDC, 0x20, 0xDB, 0x20, 0x20, 0xDB, 0x20, 0xDB, 0xDC, 0xDC, 0xDB, 0x20, 0x1B, 0x5B, 0x33, 0x34, 0x6D, 0xDB, 0x20, 0xDB, 0x20, 0xDB, 0x20, 0xDC, 0xDC, 0xDC, 0x20, 0x20, 0xDF, 0xDC, 0xDF, 0x0D, 0x0A, 0x1B, 0x5B, 0x33, 0x33, 0x6D,
    0xDB, 0x20, 0x20, 0xDB, 0x20, 0xDB, 0x20, 0x20, 0x20, 0xDB, 0x20, 0xDB, 0xDF, 0xDF, 0xDF, 0x20, 0xDB, 0x20, 0x20, 0xDB, 0x20, 0x1B, 0x5B, 0x33, 0x34, 0x6D, 0xDB, 0x20, 0x20, 0x20, 0xDB, 0x20, 0x20, 0x20, 0xDB, 0x20, 0xDC, 0xDF, 0x20,
    0xDF, 0xDC, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x1B, 0x5B, 0x31, 0x3B,
    0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0xDF, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x20, 0x20,
    0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x31, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x34, 0x6D, 0xDF, 0x1B, 0x5B, 0x30,
    0x6D, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x34, 0x6D, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x34, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x20, 0x20, 0x20, 0x1B, 0x5B, 0x31,
    0x3B, 0x33, 0x34, 0x6D, 0xDF, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x0D, 0x0A, 0x20, 0x1B, 0x5B, 0x33, 0x34, 0x6D, 0xFE, 0x1B, 0x5B, 0x31, 0x6D, 0xFE, 0x20, 0x1B, 0x5B, 0x33, 0x37, 0x6D, 0x4E, 0x6F, 0x76, 0x65, 0x6D, 0x62, 0x65, 0x72,
    0x20, 0x31, 0x33, 0x20, 0x32, 0x30, 0x32, 0x30, 0x20, 0x2D, 0x20, 0x31, 0x35, 0x20, 0x59, 0x65, 0x61, 0x72, 0x73, 0x20, 0x41, 0x6E, 0x6E, 0x69, 0x76, 0x65, 0x72, 0x73, 0x61, 0x72, 0x79, 0x1B, 0x5B, 0x30, 0x6D, 0x0D, 0x0A, 0x20, 0x1B,
    0x5B, 0x33, 0x34, 0x6D, 0xFE, 0x1B, 0x5B, 0x31, 0x6D, 0xFE, 0x20, 0x1B, 0x5B, 0x33, 0x37, 0x6D, 0x48, 0x69, 0x74, 0x20, 0x48, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x65, 0x6C, 0x65, 0x62, 0x72, 0x61, 0x74, 0x65, 0x20, 0x69, 0x74, 0x21, 0x20,
    0x4F, 0x72, 0x20, 0x77, 0x61, 0x69, 0x74, 0x20, 0x61, 0x20, 0x6C, 0x69, 0x74, 0x74, 0x6C, 0x65, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x6F, 0x6E, 0x6E, 0x65, 0x63, 0x74, 0x20, 0x74, 0x6F, 0x3A, 0x0D, 0x0A, 0x00};
// Code Page 437 font for MSX1 kindly provided by Piter Punk
const char ucCP437Font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C, 0x3C, 0x7E, 0xDB, 0xFF, 0xFF, 0xDB, 0x66, 0x3C, 0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10,
    0x00, 0x10, 0x38, 0x54, 0xFE, 0x54, 0x10, 0x38, 0x00, 0x10, 0x38, 0x7C, 0xFE, 0xFE, 0x10, 0x38, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xCF, 0xCF, 0xFF, 0xFF, 0xFF, 0x38, 0x44, 0x82, 0x82, 0x82, 0x44,
    0x38, 0x00, 0xC7, 0xBB, 0x7D, 0x7D, 0x7D, 0xBB, 0xC7, 0xFF, 0x0F, 0x03, 0x05, 0x79, 0x88, 0x88, 0x88, 0x70, 0x38, 0x44, 0x44, 0x44, 0x38, 0x10, 0x7C, 0x10, 0x30, 0x28, 0x24, 0x24, 0x28, 0x20, 0xE0, 0xC0, 0x3C, 0x24, 0x3C, 0x24, 0x24,
    0xE4, 0xDC, 0x18, 0x10, 0x54, 0x38, 0xEE, 0x38, 0x54, 0x10, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x20, 0x70, 0xF8, 0x20, 0x20, 0xF8, 0x70, 0x20, 0x50, 0x50, 0x50, 0x50,
    0x00, 0x00, 0x50, 0x00, 0x7C, 0xA8, 0xA8, 0x68, 0x28, 0x28, 0x28, 0x00, 0x38, 0x40, 0x30, 0x48, 0x48, 0x30, 0x08, 0x70, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, 0x00, 0x20, 0x70, 0xF8, 0x20, 0xF8, 0x70, 0x20, 0xF8, 0x20, 0x70, 0xF8,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x70, 0x20, 0x00, 0x00, 0x00, 0x10, 0x18, 0xFC, 0x18, 0x10, 0x00, 0x00, 0x00, 0x20, 0x60, 0xFC, 0x60, 0x20, 0x00, 0x00, 0x00, 0x80, 0x80, 0xF8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x7C, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x20, 0x00, 0x50,
    0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x50, 0x00, 0x20, 0x78, 0xA0, 0x70, 0x28, 0xF0, 0x20, 0x00, 0xC0, 0xC8, 0x10, 0x20, 0x40, 0x98, 0x18, 0x00, 0x40, 0xA0, 0x40, 0xA8, 0x90, 0x98, 0x60, 0x00,
    0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x40, 0x40, 0x40, 0x20, 0x10, 0x00, 0x40, 0x20, 0x10, 0x10, 0x10, 0x20, 0x40, 0x00, 0x20, 0xA8, 0x70, 0x20, 0x70, 0xA8, 0x20, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x70, 0x88, 0x98, 0xA8, 0xC8, 0x88,
    0x70, 0x00, 0x20, 0x60, 0xA0, 0x20, 0x20, 0x20, 0xF0, 0x00, 0x70, 0x88, 0x08, 0x10, 0x60, 0x80, 0xF8, 0x00, 0x70, 0x88, 0x08, 0x30, 0x08, 0x88, 0x70, 0x00, 0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x10, 0x00, 0xF8, 0x80, 0xE0, 0x10, 0x08,
    0x10, 0xE0, 0x00, 0x30, 0x40, 0x80, 0xF0, 0x88, 0x88, 0x70, 0x00, 0xF8, 0x88, 0x10, 0x20, 0x20, 0x20, 0x20, 0x00, 0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70, 0x00, 0x70, 0x88, 0x88, 0x78, 0x08, 0x10, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x20, 0x40, 0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0xC0, 0x60, 0x30, 0x18, 0x30, 0x60, 0xC0, 0x00, 0x70, 0x88, 0x08,
    0x10, 0x20, 0x00, 0x20, 0x00, 0x70, 0x88, 0x08, 0x48, 0xA8, 0xA8, 0x70, 0x00, 0x20, 0x50, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x00, 0xF0, 0x48, 0x48, 0x70, 0x48, 0x48, 0xF0, 0x00, 0x30, 0x48, 0x80, 0x80, 0x80, 0x48, 0x30, 0x00, 0xE0, 0x50,
    0x48, 0x48, 0x48, 0x50, 0xE0, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0x00, 0x70, 0x88, 0x80, 0xB8, 0x88, 0x88, 0x70, 0x00, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x70,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x38, 0x10, 0x10, 0x10, 0x90, 0x90, 0x60, 0x00, 0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x88, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0x00, 0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x88, 0x00,
    0x88, 0xC8, 0xC8, 0xA8, 0x98, 0x98, 0x88, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00, 0x70, 0x88, 0x88, 0x88, 0xA8, 0x90, 0x68, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0xA0, 0x90, 0x88,
    0x00, 0x70, 0x88, 0x80, 0x70, 0x08, 0x88, 0x70, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x88, 0x88, 0x88, 0x88, 0x50, 0x50, 0x20, 0x00, 0x88, 0x88, 0x88, 0xA8, 0xA8, 0xD8,
    0x88, 0x00, 0x88, 0x88, 0x50, 0x20, 0x50, 0x88, 0x88, 0x00, 0x88, 0x88, 0x88, 0x70, 0x20, 0x20, 0x20, 0x00, 0xF8, 0x08, 0x10, 0x20, 0x40, 0x80, 0xF8, 0x00, 0x70, 0x40, 0x40, 0x40, 0x40, 0x40, 0x70, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10,
    0x08, 0x04, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x80, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x08,
    0x78, 0x88, 0x78, 0x00, 0x80, 0x80, 0xB0, 0xC8, 0x88, 0xC8, 0xB0, 0x00, 0x00, 0x00, 0x70, 0x88, 0x80, 0x88, 0x70, 0x00, 0x08, 0x08, 0x68, 0x98, 0x88, 0x98, 0x68, 0x00, 0x00, 0x00, 0x70, 0x88, 0xF8, 0x80, 0x70, 0x00, 0x10, 0x28, 0x20,
    0xF8, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x68, 0x98, 0x98, 0x68, 0x08, 0x70, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0x88, 0x00, 0x20, 0x00, 0x60, 0x20, 0x20, 0x20, 0x70, 0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 0x90, 0x60, 0x40, 0x40,
    0x48, 0x50, 0x60, 0x50, 0x48, 0x00, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00, 0x00, 0xD0, 0xA8, 0xA8, 0xA8, 0xA8, 0x00, 0x00, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00,
    0x00, 0xB0, 0xC8, 0xC8, 0xB0, 0x80, 0x80, 0x00, 0x00, 0x68, 0x98, 0x98, 0x68, 0x08, 0x08, 0x00, 0x00, 0xB0, 0xC8, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x78, 0x80, 0x70, 0x08, 0xF0, 0x00, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x48, 0x30, 0x00,
    0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x68, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x50, 0x20, 0x00, 0x00, 0x00, 0x88, 0xA8, 0xA8, 0xA8, 0x50, 0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x88, 0x88, 0x98, 0x68, 0x08,
    0x70, 0x00, 0x00, 0xF8, 0x10, 0x20, 0x40, 0xF8, 0x00, 0x18, 0x20, 0x20, 0x40, 0x20, 0x20, 0x18, 0x00, 0x20, 0x20, 0x20, 0x00, 0x20, 0x20, 0x20, 0x00, 0xC0, 0x20, 0x20, 0x10, 0x20, 0x20, 0xC0, 0x00, 0x40, 0xA8, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x50, 0xF8, 0x00, 0x00, 0x00, 0x30, 0x48, 0x80, 0x80, 0x80, 0x48, 0x30, 0x60, 0x50, 0x00, 0x90, 0x90, 0x90, 0x90, 0x68, 0x00, 0x10, 0x20, 0x70, 0x88, 0xF8, 0x80, 0x70, 0x00, 0x20, 0x50, 0x70, 0x08, 0x78,
    0x88, 0x78, 0x00, 0x50, 0x00, 0x70, 0x08, 0x78, 0x88, 0x78, 0x00, 0x40, 0x20, 0x70, 0x08, 0x78, 0x88, 0x78, 0x00, 0x20, 0x50, 0x20, 0x70, 0x08, 0xF8, 0x78, 0x00, 0x00, 0x00, 0x70, 0x88, 0x80, 0x88, 0x70, 0x20, 0x20, 0x50, 0x70, 0x88,
    0xF8, 0x80, 0x70, 0x00, 0x50, 0x00, 0x70, 0x88, 0xF8, 0x80, 0x70, 0x00, 0x40, 0x20, 0x70, 0x88, 0xF8, 0x80, 0x70, 0x00, 0x50, 0x00, 0x00, 0x60, 0x20, 0x20, 0x70, 0x00, 0x20, 0x50, 0x00, 0x60, 0x20, 0x20, 0x70, 0x00, 0x40, 0x20, 0x00,
    0x60, 0x20, 0x20, 0x70, 0x00, 0x50, 0x00, 0x20, 0x50, 0x88, 0xF8, 0x88, 0x00, 0x20, 0x50, 0x20, 0x50, 0x88, 0xF8, 0x88, 0x00, 0x10, 0x20, 0xF8, 0x80, 0xF0, 0x80, 0xF8, 0x00, 0x00, 0x00, 0x6C, 0x12, 0x7E, 0x90, 0x6E, 0x00, 0x3E, 0x50,
    0x90, 0x9C, 0xF0, 0x90, 0x9E, 0x00, 0x20, 0x50, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x50, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x40, 0x20, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x50, 0x00, 0x90, 0x90, 0x90, 0x68, 0x00, 0x40,
    0x20, 0x00, 0x90, 0x90, 0x90, 0x68, 0x00, 0x50, 0x00, 0x88, 0x88, 0x98, 0x68, 0x08, 0x70, 0x50, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x50, 0x00, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x20, 0x20, 0x78, 0x80, 0x80, 0x78, 0x20, 0x20,
    0x18, 0x24, 0x20, 0xF8, 0x20, 0xE2, 0x5C, 0x00, 0x88, 0x50, 0x20, 0xF8, 0x20, 0xF8, 0x20, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x88, 0x9C, 0x88, 0x0C, 0x18, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x20, 0x40, 0x10, 0x20, 0x70, 0x08, 0x78, 0x88, 0x78,
    0x00, 0x10, 0x20, 0x00, 0x60, 0x20, 0x20, 0x70, 0x00, 0x10, 0x20, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x10, 0x20, 0x90, 0x90, 0x90, 0x90, 0x68, 0x00, 0x28, 0x50, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x00, 0x28, 0x50, 0x88, 0xC8, 0xA8, 0x98,
    0x88, 0x00, 0x60, 0x90, 0x90, 0x68, 0x00, 0xF8, 0x00, 0x00, 0x60, 0x90, 0x90, 0x60, 0x00, 0xF0, 0x00, 0x00, 0x20, 0x00, 0x20, 0x40, 0x80, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x08,
    0x08, 0x00, 0x00, 0x84, 0x88, 0x90, 0xA8, 0x54, 0x84, 0x08, 0x1C, 0x84, 0x88, 0x90, 0xA8, 0x58, 0xA8, 0x3C, 0x08, 0x20, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x12, 0x24, 0x48, 0x90, 0x48, 0x24, 0x12, 0x00, 0x90, 0x48, 0x24,
    0x12, 0x24, 0x48, 0x90, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0xF0, 0x10, 0x10, 0x10, 0x28, 0x28, 0x28, 0xE8, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0xF8, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0xF0, 0x10, 0xF0, 0x10, 0x10, 0x10, 0x28, 0x28,
    0xE8, 0x08, 0xE8, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00, 0xF8, 0x08, 0xE8, 0x28, 0x28, 0x28, 0x28, 0x28, 0xE8, 0x08, 0xF8, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x10, 0xF0, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x28, 0x28, 0x28, 0x2F, 0x28, 0x28, 0x28,
    0x28, 0x28, 0x28, 0x2F, 0x20, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x20, 0x2F, 0x28, 0x28, 0x28, 0x28, 0x28, 0xEF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xEF, 0x28, 0x28, 0x28, 0x28, 0x28, 0x2F, 0x20, 0x2F, 0x28,
    0x28, 0x28, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x28, 0x28, 0xEF, 0x00, 0xEF, 0x28, 0x28, 0x28, 0x10, 0x10, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF,
    0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0xFF, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x3F,
    0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xEF, 0x28, 0x28, 0x28, 0x28, 0x10, 0x10, 0xFF, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x68, 0x90, 0x90, 0x90, 0x68, 0x00, 0x30, 0x48, 0x48, 0x70, 0x48, 0x48, 0x70, 0xC0, 0xF8, 0x88, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xF8, 0x50, 0x50, 0x50, 0x50, 0x50, 0x98, 0x00, 0xF8, 0x88, 0x40, 0x20, 0x40, 0x88, 0xF8, 0x00, 0x00,
    0x00, 0x78, 0x90, 0x90, 0x90, 0x60, 0x00, 0x00, 0x50, 0x50, 0x50, 0x50, 0x68, 0x80, 0x80, 0x00, 0x50, 0xA0, 0x20, 0x20, 0x20, 0x20, 0x00, 0xF8, 0x20, 0x70, 0xA8, 0xA8, 0x70, 0x20, 0xF8, 0x20, 0x50, 0x88, 0xF8, 0x88, 0x50, 0x20, 0x00,
    0x70, 0x88, 0x88, 0x88, 0x50, 0x50, 0xD8, 0x00, 0x30, 0x40, 0x40, 0x20, 0x50, 0x50, 0x50, 0x20, 0x00, 0x00, 0x00, 0x50, 0xA8, 0xA8, 0x50, 0x00, 0x08, 0x70, 0xA8, 0xA8, 0xA8, 0x70, 0x80, 0x00, 0x38, 0x40, 0x80, 0xF8, 0x80, 0x40, 0x38,
    0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 0xF8, 0x00, 0xC0, 0x30, 0x08, 0x30, 0xC0, 0x00, 0xF8, 0x00, 0x18, 0x60, 0x80, 0x60, 0x18, 0x00,
    0xF8, 0x00, 0x10, 0x28, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA0, 0x40, 0x00, 0x20, 0x00, 0xF8, 0x00, 0x20, 0x00, 0x00, 0x00, 0x50, 0xA0, 0x00, 0x50, 0xA0, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x30, 0x78, 0x78, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x3E, 0x20, 0x20, 0x20, 0xA0, 0x60, 0x20, 0x00, 0xA0, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x40, 0xA0, 0x20, 0x40,
    0xE0, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Our Flags
unsigned char ucEcho;                 // Echo On?
unsigned char ucAutoDownload;         // Auto download on binary transfers?
unsigned char ucAnsi;                 // Using ANSI rendering?
unsigned char ucCP437;                // Using custom CP437 if MSX1?
unsigned char ucEnterHit;             // user has input enter?
unsigned char ucWidth40;              // Detected 40 Columns or less?
unsigned char ucState;                // State of Telnet Data Parser
unsigned char ucCmdCounter;           // If there is a TELNET command in progress, its size
unsigned char ucStandardDataTransfer; // Is this telnet server proper and transmitting files using telnet double FF?
unsigned char ucConnNumber;           // hold the connection number received by UnapiHelper

// For data receive parsing
unsigned char ucRcvData[128];

// MSX Variables that we will access
__at 0xF3B0 unsigned char ucLinLen;
__at 0xFCA9 unsigned char ucCursorDisplayed;
__at 0xFBEB unsigned char ucMT6;    // Current state of keyboard matrix for F3 F2 F1 CODE CAP GRAPH CTRL SHIFT
__at 0xFBEC unsigned char ucMT7;
__at 0xFC9E unsigned int  uiJiffy;

// IMPORTANT: You need to check the map compiler generates to make sure this
// address do not overlap functions, variables, etc
// UNAPI requires memory buffer @ 0x8000 or higher...
#define RcvMemorySize 1024
unsigned char ucRcvDataMemory[RcvMemorySize]; // area to hold data sent to Fossil driver, avoid 0x8000 as it seems somehow it is used

unsigned int uiGetSize;

Z80_registers regs; // auxiliary structure for asm function calling

void         negotiate(unsigned char *ucBuf);
unsigned int IsValidInput(char **argv, int argc, unsigned char *ucServer, unsigned char *ucPort, unsigned char *ucAnsiOption, unsigned char *ucMSX1CustomFont);
void         ParseTelnetData(void);
void         SendCursorPosition(unsigned int uiCursorPosition) __z88dk_fastcall;
#endif // _TELNET_HEADER_INCLUDED
