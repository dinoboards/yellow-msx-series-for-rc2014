#include "aofossilhelper.h"
#include "fossil_interface.h"
#include "msx_fusion.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Breath() { return; }

unsigned char InitializeTCPIP() { return 1; }

unsigned char OpenSingleConnection(unsigned char *uchHost, unsigned char *uchPort, unsigned char *uchConn) {
  (void)uchHost;
  (void)uchPort;
  (void)uchConn;
  return ERR_OK;
}

unsigned char CloseConnection(unsigned char ucConnNumber) {
  (void)ucConnNumber;

  return 0;
}

unsigned char IsConnected(unsigned char ucConnNumber) {
  (void)ucConnNumber;
  return 1;
}

unsigned char RXData(unsigned char ucConnNumber, unsigned char *ucBuffer, unsigned int *uiSize, unsigned char ucWaitAllDataReceived) {
  (void)ucConnNumber;
  (void)ucBuffer;
  (void)uiSize;
  (void)ucWaitAllDataReceived;

  return 'a';
}

// This routine sends only one byte
unsigned char TxByte(unsigned char ucConnNumber, unsigned char uchByte) { return TxData(ucConnNumber, &uchByte, 1); }

unsigned char TxUnsafeData(unsigned char ucConnNumber, unsigned char *lpucData, unsigned int uiDataSize) { return TxData(ucConnNumber, lpucData, uiDataSize); }

// The same as TxUnsafeData but without page 3 buffer addressing
unsigned char TxData(unsigned char ucConnNumber, unsigned char *lpucData, unsigned int uiDataSize) {
  (void)ucConnNumber;
  (void)lpucData;
  (void)uiDataSize;

  return ERR_OK;
}
