#ifndef CP2112_H
#define CP2112_H

#include "types.h"
#include "SLABCP2112.h"

#define VID 0x10C4
#define PID 0xEA90

INT CP2112_Find();
INT CP2112_Open(INT DevNumber, HID_SMBUS_DEVICE *device);
INT CP2112_SetConfig(HID_SMBUS_DEVICE device);
INT CP2112_GetGpioConfig(HID_SMBUS_DEVICE device, BYTE *direction, BYTE *mode, BYTE *function);
INT CP2112_SetGpioConfig(HID_SMBUS_DEVICE device);


#endif // CP2112_H
