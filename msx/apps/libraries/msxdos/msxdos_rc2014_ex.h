
#ifndef _MSXDOS_RC2014_EX_H_
#define _MSXDOS_RC2014_EX_H_

typedef struct {
  uint8_t extendedMediumType;

  /*
  Bit 0 -> IS USB
  */
  uint8_t extendedFlags;

  union {
    struct {
      uint8_t address;
      uint8_t configIndex;
      uint8_t interfaceIndex;

    } usbInfo;
  } extendedInfo;

} rc2014LunInfoExtended;

extern uint16_t rc2014GetLunInfoEx(const uint8_t slot_id, const uint8_t index, const uint8_t lun, rc2014LunInfoExtended *info);

#endif
