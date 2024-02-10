#include "msxhub.h"
#include "arguments.h"
#include "esp8266.h"
#include "print.h"
#include "wget.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <xmodem/buffer.h>
#include <xmodem/file_storage.h>
#include <xmodem/receive.h>

#define packetBody     (write_buffer)
#define packetBodySize (DATA_SIZE)

char fileNames[1025];
char msxHubUrl[128];
char msxHubFileName[13];

char *curLine;
char *nextLinePtr;
char  lineBuffer[128];
char *pStart;

static uint32_t retrieved_bytes;

const char MSX_HUB_API_PREFIX[] = "https://msxhub.com/api/";

char *str_append(char *dest, const char *src) {
  while (*src)
    *dest++ = *src++;

  *dest = 0;
  return dest;
}

char *str_append_upper(char *dest, const char *src) {
  while (*src)
    *dest++ = toupper(*src++);

  *dest = 0;
  return dest;
}

char *currentLine(void) {
  if (curLine) {
    nextLinePtr = (char *)memchr(curLine, '\r', 1024 - (curLine - pStart));
    if (!nextLinePtr)
      nextLinePtr = (char *)memchr(curLine, CTRLZ, 1024 - (curLine - pStart));
    const int length = nextLinePtr ? nextLinePtr - curLine : 1025 - curLine;
    memcpy(lineBuffer, curLine, length);
    lineBuffer[length] = 0;

    if (*nextLinePtr == CTRLZ)
      nextLinePtr = NULL;

    return lineBuffer;
  }

  return NULL;
}

char *eachLine(char *p) __z88dk_fastcall {
  pStart      = p;
  curLine     = p;
  nextLinePtr = NULL;

  return currentLine();
}

void nextLine(void) { curLine = nextLinePtr ? (nextLinePtr + 2) : NULL; }

int8_t getNumberOfPages(void) {
  msxHubUrl[0] = 0;
  char *p      = str_append(msxHubUrl, MSX_HUB_API_PREFIX);
  p            = str_append_upper(p, pMsxHubPackageName);
  p            = str_append(p, "/latest/pages");

  pWgetUrl      = msxHubUrl;
  pFilePathName = NULL;

  printf("Retrieving File List Page Count ");

  if (wget_retry(&retrieved_bytes, 3))
    abortWithError("Unable to retrieve number of pages");

  const uint8_t count = atoi(packetBody);

  printf(ERASE_LINE "File List Page Count: %d\r\n", count);

  return count;
}

void getFileListForPage(int8_t i) __z88dk_fastcall {
  msxHubUrl[0] = 0;
  char *p      = str_append(msxHubUrl, MSX_HUB_API_PREFIX);
  p            = str_append_upper(p, pMsxHubPackageName);
  p            = str_append(p, "/latest/files/");
  sprintf(p, "%d", i);

  pWgetUrl      = msxHubUrl;
  pFilePathName = NULL;

  printf("Retrieving file list page %d ", i);

  if (wget_retry(&retrieved_bytes, 3))
    abortWithError("Unable to retrieve file list page");

  printf(ERASE_LINE "Retrieved Page %d file list", i);

  printf("\r\n");
}

const char *getInstallDirectory(void) {
  msxHubUrl[0] = 0;
  char *p      = str_append(msxHubUrl, MSX_HUB_API_PREFIX);
  p            = str_append_upper(p, pMsxHubPackageName);
  str_append(p, "/latest/installdir");

  pWgetUrl      = msxHubUrl;
  pFilePathName = NULL;

  printf("Retrieving install directory ");

  if (wget_retry(&retrieved_bytes, 3))
    abortWithError("Unable to retrieve install directory");

  char *stringEndPtr = (char *)memchr(packetBody, CTRLZ, DATA_SIZE);
  *stringEndPtr      = 0;

  printf(ERASE_LINE "Installing into directory: '%s'\r\n", packetBody + 1);

  return packetBody + 1; // skip leading slash
}

void createInstallDirectory(const char *installDir) __z88dk_fastcall {
  /*int result =*/mkdir((char *)installDir, S_IRWXO);

  // if (result)
  //   abortWithError("Unable to create install directory");

  chdir(installDir);
}

void downloadFile(const char *fileName) __z88dk_fastcall {
  msxHubUrl[0] = 0;
  char *p      = str_append(msxHubUrl, MSX_HUB_API_PREFIX);
  p            = str_append_upper(p, pMsxHubPackageName);
  p            = str_append(p, "/latest/get/");
  p            = str_append_upper(p, pMsxHubPackageName);
  p            = str_append(p, "/");
  str_append(p, fileName);

  pWgetUrl      = msxHubUrl;
  pFilePathName = fileName;

  printf("Downloading '%s' ", fileName);

  if (wget_retry(&retrieved_bytes, 3))
    abortWithError("Unable to retrieve file");

  printf(ERASE_LINE "Downloaded '%s' (%ld bytes)\r\n", fileName, retrieved_bytes);
}

void subCommandMsxHub(void) {
  const char *installDir = getInstallDirectory();
  createInstallDirectory(installDir);

  const int p = getNumberOfPages();

  for (int i = 0; i < p; i++) {
    getFileListForPage(i + 1);

    memcpy(fileNames, packetBody, DATA_SIZE);

    int ii = 20;
    for (char *line = eachLine(fileNames); line = currentLine(); nextLine()) {
      downloadFile(line);

      if (ii-- == 0)
        break;
    }
  }
}
