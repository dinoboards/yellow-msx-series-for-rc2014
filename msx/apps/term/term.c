/*
--
-- telnet.c
--   Simple TELNET client using UNAPI for MSX.
--   Also supports Andres Ortiz ESP8266 BaDCaT modem
--   Revision 1.34
--
-- Requires SDCC and Fusion-C library to compile
-- Copyright (c) 2019 - 2020 Oduvaldo Pavan Junior ( ducasp@gmail.com )
-- Copyright (c) 2020 Andres Ortiz for serial interface using Fossil driver
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
#include "term.h"
#include "ansi_codes.h"
#include "ansiprint.h"
#include "aofossilhelper.h"
#include "arguments.h"
#include "msxdos.h"
#include "xymodem.h"
#include <extbio.h>
#include <system_vars.h>

// Those won't change, so we won't waste memory and use global constants
const unsigned char ucWindowSize[] = {IAC, WILL, CMD_WINDOW_SIZE, IAC, SB, CMD_WINDOW_SIZE, 0, 80, 0, 24, IAC, SE};  // our terminal is 80x24
const unsigned char ucWindowSize0[] = {IAC, WILL, CMD_WINDOW_SIZE, IAC, SB, CMD_WINDOW_SIZE, 0, 40, 0, 24, IAC, SE}; // our terminal is 40x24
const unsigned char ucWindowSize1[] = {IAC, WILL, CMD_WINDOW_SIZE, IAC, SB, CMD_WINDOW_SIZE, 0, 80, 0, 25, IAC, SE}; // our terminal is 80x25
const unsigned char ucTTYPE2[] = {IAC, SB, CMD_TTYPE, IS, 'A', 'N', 'S', 'I', IAC, SE};                              // Terminal ANSI
const unsigned char ucTTYPE3[] = {IAC, SB, CMD_TTYPE, IS, 'V', 'T', '5', '2', IAC, SE};                              // Terminal UNKNOWN

char           ucTxData = 0; // where our key inputs go
unsigned char  ucRet;        // return of functions
char           chTextLine[128];
unsigned char  ucCursorSave;
unsigned char  ucFnkBackup[160];
unsigned char *ucFnkStr = (unsigned char *)0xF87F;
unsigned char  ucF5Exit = 0;
unsigned char  ucUseCrLf = 0;
unsigned char  ucLockF2 = 0;
unsigned char  ucLockF3 = 0;

// This will handle CMD negotiation...
// Basically, the first time host send any command our client will send it
// is willing to send the following information:
// Terminal Type (ANSI if on MSX 2 or better or UNKNOWN if plain text / MSX1)
// Window Size (80x25 if ANSI running or 80x24 / 40x24 if MSX1)
//
// Upon receiving a DO CMD_WINDOW_SIZE, respond with Window Size.
// Upon receiving TTYPE SUB OPTION request, respond accordingly whether dumb
// or ANSI.
//
// Upon receiving WILL ECHO, turn off our ECHO (as host will ECHO), otherwise
// if receiving WONT ECHO or no ECHO negotiation, we will ECHO locally.
// Upon receiving TRANSMIT_BINARY, will just acknowledge if enter was not hit
// as some BBSs send it at start negotiations, but after that, invoke the
// file transfer function automatically (this works only on Synchronet BBSs)
//
// Treat the DO for TTYPE with an WILL to tell that we are ready to send the
// information when requested.
//
// Any other negotiation requested will be replied as:
// Host asking us if we can DO something are replied as WONT do it
// Host telling that it WILL do something, we tell it to DO it
void negotiate(unsigned char *ucBuf) {
  switch (ucBuf[1]) {
  case DO:
    switch (ucBuf[2]) {
    case CMD_WINDOW_SIZE:
      // request of our terminal window size
      if (ucAnsi)
        TxUnsafeData(ucWindowSize1, sizeof(ucWindowSize1)); // 80x25
      else if (!ucWidth40)
        TxUnsafeData(ucWindowSize, sizeof(ucWindowSize)); // 80x24
      else
        TxUnsafeData(ucWindowSize0, sizeof(ucWindowSize0)); // 40x24
      break;
    // we are willing to negotiate TTYPE and TERMINAL SPEED
    case CMD_TTYPE:
    case CMD_TRANSMIT_BINARY:
      ucBuf[1] = WILL;
      TxUnsafeData(ucBuf, 3);
      break;
    default:
      ucBuf[1] = WONT;
      TxUnsafeData(ucBuf, 3);
      break;
    }
    break;
  case WILL:
    switch (ucBuf[2]) {
    case CMD_ECHO:
      // Host is going to echo
      ucEcho = 0;
      ucBuf[1] = DO;
      TxUnsafeData(ucBuf, 3); // Ok host, you can echo, I'm not going to echo
      break;
    case CMD_TRANSMIT_BINARY:
      // Ok, can do it
      ucBuf[1] = DO;
      TxUnsafeData(ucBuf, 3);
      // Initial handshake?
      if (!ucEnterHit)
        // If we received TRANSMIT BINARY right at the beginning, odds are that this BBS
        // will not use it to signal file transfers
        ucAutoDownload = 0;
      // Host is going to send a file?
      else if (ucAutoDownload)
        // Some BBSs use transmit binary at start of telnet negotiations, thus why not do
        // this before ENTER is HIT (which is Ok, ANSI data is 8 bit)
        //
        // Also some BBBs use it before transmitting any data, and those that do will do
        // it during initial hand-shake, before user type anything. In this case AutoDownload
        // should disable the download detection (which works for Syncrhonet BBSs just fine,
        // and Synchronet BBSs do not sent it during initial handshake)
        XYModemGet(ucStandardDataTransfer);
      break;
    default:
      ucBuf[1] = DO;
      TxUnsafeData(ucBuf, 3);
      break;
    }
    break;
  case SB:
    if (ucBuf[2] == CMD_TTYPE) {
      // requesting Terminal Type list
      if (ucAnsi)
        TxUnsafeData(ucTTYPE2, sizeof(ucTTYPE2)); // ANSI
      else
        TxUnsafeData(ucTTYPE3, sizeof(ucTTYPE3)); // dumb/unknown
    }
    break;
  case WONT:
    if (ucBuf[2] == CMD_ECHO)
      // Host is not going to echo
      ucEcho = 1;
    ucBuf[1] = DONT;
    TxUnsafeData(ucBuf, 3);
    break;
  case DONT:
    ucBuf[1] = WONT;
    TxUnsafeData(ucBuf, 3);
    break;
  }
}

// This function will handle a received buffer from a TELNET connection. If
// there are TELNET commands or sub-commands, it will properly remove and not
// print those, as well try to negotiate it using our negotiate function.
// Also clear double FF's (this is how telnet indicate FF) and replace by a
// single FF.
void ParseTelnetData() {
  unsigned char *chTmp = ucRcvDataMemory;
  unsigned char *chLimit = chTmp + uiGetSize;

  do {
    switch (ucState) {
    case TELNET_IDLE:
      // No command is being built, so is the character IAC?
      if (*chTmp == IAC) {
        ucRcvData[0] = *chTmp;           // copy to command buffer
        ucState = TELNET_CMD_INPROGRESS; // flag a command or sub is in progress
        ucCmdCounter = 1;
        ++chTmp;
      } else {
        // Do it until IAC or hit the limit
        for (; (chTmp < chLimit) && (*chTmp != IAC); ++chTmp)
          printChar(*chTmp);
      }
      break;
    case TELNET_CMD_INPROGRESS:
      // Get the byte in the proper position
      ucRcvData[ucCmdCounter] = *chTmp;
      // Is it the first byte after IAC? If yes and it is IAC again
      if ((ucCmdCounter == 1) && (*chTmp == IAC)) {
        ++chTmp;               // skip current FF
        ucState = TELNET_IDLE; // CMD finished
        printChar(0xff);
      }
      // Is it a two byte command? Just ignore, we do not react to those
      else if ((ucCmdCounter == 1) && (*chTmp <= GA) && (*chTmp >= NOP)) {
        ++chTmp;               // jump the CMD
        ucState = TELNET_IDLE; // CMD finished
      }
      // Is it the first byte after IAC and now indicate a sub option?
      else if ((ucCmdCounter == 1) && (*chTmp == SB)) {
        // ok, it is a sub option, keep going
        ucState = TELNET_SUB_INPROGRESS; // suboption state update
        ++ucCmdCounter;                  // cmd size update
        ++chTmp;
      } else // ok, nothing special, just data for IAC or SUB
      {
        ++ucCmdCounter;
        ++chTmp;
        if (ucCmdCounter == 3) {
          ucState = TELNET_IDLE; // CMD finished
          // Negotiate the sub option
          negotiate(ucRcvData);
        }
      }
      break;
    case TELNET_SUB_INPROGRESS:
      // Get the byte in the proper position
      ucRcvData[ucCmdCounter] = *chTmp;
      // If receive IAC processing sub option, it could be the end of sub option
      if (*chTmp == IAC) // need to wait for IAC /SE
      {
        ucState = TELNET_SUB_WAITEND;
        ++ucCmdCounter; // cmd size update
        ++chTmp;
      } else // ok, nothing special, just data for IAC or SUB
      {
        ++ucCmdCounter;
        ++chTmp;
      }
      break;
    case TELNET_SUB_WAITEND:
      // Get the byte in the proper position
      ucRcvData[ucCmdCounter] = *chTmp;
      // is the next byte SE?
      if (*chTmp == SE) {
        ++chTmp;               // skip SE
        ucState = TELNET_IDLE; // CMD finished
        // Negotiate the sub option
        negotiate(ucRcvData);
      }
      // Was processing sub option, received IAC, but now it is not SE
      else {
        // Keep processing sub option, not the end
        ucState = TELNET_SUB_WAITEND;
        ++ucCmdCounter; // cmd state update
        ++chTmp;
      }
      break;
    }
  } while (chTmp < chLimit); // While we have data in the buffer
}

// This is a callback function
// MSX2ANSI will call this function when ESC[6n is received
// And we will send the current cursor position over the connection
// This is crucial for quite a few BBSs terminal window size detection routines
// As well Synchronet BBSs that have avatars that will be misplaced without this
void SendCursorPosition(unsigned int uiCursorPosition) __z88dk_fastcall {
  unsigned char uchPositionResponse[12];
  unsigned char uchRow, uchColumn;

  uchColumn = uiCursorPosition & 0xff;
  uchRow = (uiCursorPosition >> 8) & 0xff;
  // return cursor position
  sprintf(uchPositionResponse, "\x1b[%u;%uR", uchRow, uchColumn);
  TxUnsafeData(uchPositionResponse, strlen((char *)uchPositionResponse));
}

extern void debugBreak();

int main(const int argc, const unsigned char **argv) {
  process_cli_arguments(argc, argv);

  extbio_fossil_install();

  // we detect if enter was hit to avoid popping up protocol selection if transmit binary command is received in initial negotiations
  ucEnterHit = 0;
  // no bytes received yet
  uiGetSize = 0;
  // save cursor status
  ucCursorSave = CSRSW;

  // Telnet Protocol Flags
  // Flag that indicates that a SUB OPTION reception is in progress
  ucState = TELNET_IDLE;
  // If server do not negotiate, we won't echo, adapter takes care for us
  ucEcho = 0;

  initPrint();

  // Initialize our text print engine
  ucWidth40 = 0;
  if (ucAnsi)
    initAnsi((unsigned int)SendCursorPosition);
  else {
    LINL40 = 80;
    msxbiosInitxt();
  }

  // Time to check for TCPIP availability
  if (!InitializeTCPIP()) {
    if (ucAnsi) // Using MSX2ANSI?
      endAnsi();

    printf("Unable to connect to serial port\r\n");
    // restore cursor status
    CSRSW = ucCursorSave;
    return 0;
  }

  // Backup function keys
  memcpy(ucFnkBackup, ucFnkStr, 160);
  // Make sure those won't have any text
  memset(ucFnkStr, '\0', 160);

  if (ucAnsi)
    print("\x1b[32mTerm Ready\x1b[0m\r\n\r\n");
  else
    print("Term Ready\r\n\r\n");

  do {
    if (msxbiosBreakX()) {
      ucF5Exit = true;
      break;
    }

    Breath();

    if ((NEWKEY[6] & 0x21) == 1)          // F1 and not shift: Start Transfer
      XYModemGet(ucStandardDataTransfer); // no need to lock, function will wait for key input

    if ((NEWKEY[6] & 0x41) == 1)               // F2 and not shift: Change Echo
      ucLockF2 = 1;                            // key pressed, wait until it is released
    else if ((ucLockF2) && (NEWKEY[6] & 0x40)) // key released, let's do it
    {
      ucEcho = !ucEcho;
      ucLockF2 = 0;
    }

    if ((NEWKEY[6] & 0x81) == 1)               // F3 and not shift: Change Cr / CrLf
      ucLockF3 = 1;                            // key pressed, wait until it is released
    else if ((ucLockF3) && (NEWKEY[6] & 0x80)) // key released, let's do it
    {
      ucUseCrLf = !ucUseCrLf;
      ucLockF3 = 0;
    }

    if ((!(NEWKEY[7] & 0x2)) && ((NEWKEY[6] & 0x1))) // F5 and not shift: Exit
    {
      // no need to lock, exit immediately
      ucF5Exit = 1;
      break;
    }

    ucTxData = Inkey();
    // A key has been hit?
    if (ucTxData) {
      if (ucTxData == 13) // enter/CR ?
      {
        if (ucUseCrLf)
          // Send CR and LF as well
          TxUnsafeData("\r\n", 2);
        else // just send cr
          TxByte(ucTxData);
        // Update flag that enter has been hit
        ucEnterHit = 1;
      } else if (ucTxData == 28) // right?
        TxUnsafeData(CURSOR_FORWARD, 3);
      else if (ucTxData == 29) // left?
        TxUnsafeData(CURSOR_BACKWARD, 3);
      else if (ucTxData == 30) // up?
        TxUnsafeData(CURSOR_UP, 3);
      else if (ucTxData == 31) // down?
        TxUnsafeData(CURSOR_DOWN, 3);
      else
        // Send the byte directly
        TxByte(ucTxData);

      // If we are echoing our own keys
      if (ucEcho) {
        if (ucTxData != 13)
          printChar(ucTxData);
        else
          print("\r\n");
      }
    }

    // Is there DATA?
    uiGetSize = RcvMemorySize;
    if (RXData(ucRcvDataMemory, &uiGetSize, 0)) {
      // Data received?
      if (uiGetSize) {
        // Warn we are going to print a whole buffer
        StartPrintBuffer();
        // Parse it and do what is needed, including printing it
        ParseTelnetData();
        // Buffer Processing finished
        EndPrintBuffer();
      }
    }
  } while (1);

  if (ucAnsi)  // using msx2ansi?
    endAnsi(); // terminate its screen mode

  if (ucF5Exit)                         // F5 pressed?
    print("Closing connection...\r\n"); // Yes, so we are closing
  else
    print("Connection closed on the other end...\r\n"); // No, so we will try to close after the other end closed
  CloseConnection();

  // restore cursor status
  CSRSW = ucCursorSave;
  // restore function keys
  memcpy(ucFnkStr, ucFnkBackup, 160);

  return 0;
}
