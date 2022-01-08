#ifndef BATTERYCOMMANDS_H
#define BATTERYCOMMANDS_H

#include "types.h"
#include "SLABCP2112.h"
#include "SMBusConfig.h"

UINT16 ReadWord(HID_SMBUS_DEVICE device, INT *data, INT battery_register);
UINT16 ReadBlock(HID_SMBUS_DEVICE device, char *block, INT *block_length, INT battery_register);
UINT16 ReadDataBlock(HID_SMBUS_DEVICE device, BYTE *block, INT *block_length, INT battery_register);

#endif // BATTERYCOMMANDS_H
