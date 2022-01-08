#include "types.h"
#include "SLABCP2112.h"



INT SMBus_Open(HID_SMBUS_DEVICE *device);
INT SMBus_Close(HID_SMBUS_DEVICE device);
INT SMBus_Reset(HID_SMBUS_DEVICE device);
INT SMBus_Configure(HID_SMBUS_DEVICE device, DWORD bitRate, BYTE address, BOOL autoReadRespond, WORD writeTimeout, WORD readTimeout, BOOL sclLowTimeout, WORD transferRetries, DWORD responseTimeout);
INT SMBus_Read(HID_SMBUS_DEVICE device, BYTE *buffer, BYTE slaveAddress, WORD numBytesToRead, BYTE targetAddressSize, BYTE *targetAddress);
INT SMBus_Write(HID_SMBUS_DEVICE device, BYTE *buffer, BYTE slaveAddress, BYTE numBytesToWrite);
