#include "cp2112.h"
#include "SMBusConfig.h"


INT CP2112_Find()
{
    DWORD       numDevices=-1;     //Количество найденных устройств

    HidSmbus_GetNumDevices(&numDevices, VID, PID);
    return numDevices;
}

INT CP2112_Open(INT devNumber, HID_SMBUS_DEVICE *device)
{
    BOOL                    opened;         //Открылось ли устройство
    HID_SMBUS_STATUS        status;

    status = HidSmbus_Open(device, devNumber, VID, PID);
    if (status != HID_SMBUS_SUCCESS) return FALSE;
    status = HidSmbus_IsOpened(*device, &opened);
    if (status != HID_SMBUS_SUCCESS) return FALSE;
    if (!opened)return FALSE;
    return TRUE;   // Успех!
}

INT CP2112_SetConfig(HID_SMBUS_DEVICE device)
{
    BOOL                opened;
    HID_SMBUS_STATUS    status;

    // Make sure that the device is opened
    if(HidSmbus_IsOpened(device, &opened) == HID_SMBUS_SUCCESS && opened)
    {
        // Attempt configuration
        status =  HidSmbus_SetSmbusConfig(device, BITRATE_HZ, ACK_ADDRESS, FALSE, WRITE_TIMEOUT_MS, READ_TIMEOUT_MS, SCL_LOW_TIMEOUT, TRANSFER_RETRIES);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return FALSE;
        }

        // Set response timeout
        status = HidSmbus_SetTimeouts(device, RESPONSE_TIMEOUT_MS);
        // Check status
        if(status != HID_SMBUS_SUCCESS)
        {
            return FALSE;
        }
    }

    return TRUE;
}
