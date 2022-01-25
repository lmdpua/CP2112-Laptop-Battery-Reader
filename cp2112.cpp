#include "cp2112.h"
#include "SMBusConfig.h"
#include "GPIO_Config.h"

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

INT CP2112_GetGpioConfig(HID_SMBUS_DEVICE device, BYTE *direction, BYTE *mode, BYTE *function)
{
    BOOL opened;

        // Make sure that the device is opened
        if (HidSmbus_IsOpened(device, &opened) == HID_SMBUS_SUCCESS && opened)
        {
            BYTE m_clkDiv;

            // Get GPIO direction and mode bitmasks
            HID_SMBUS_STATUS status = HidSmbus_GetGpioConfig(device, direction, mode, function, &m_clkDiv);

            if (status == HID_SMBUS_SUCCESS)
            {
                return TRUE;
            }
                return FALSE;
        }
        return FALSE;
}

INT CP2112_SetGpioConfig(HID_SMBUS_DEVICE device)
{
    BYTE    direction   = 0x00; //is a bitmask that specifies each GPIO pin’s direction.
    BYTE    mode        = 0x00; //is a bitmask that specifies each GPIO pin’s mode.
    BYTE    function    = 0x00; //is a bitmask that specifies the special behavior of GPIO.0, GPIO.1, and GPIO.7
    BYTE    m_clkDiv    = 0x00; //is the clock output divider value used for GPIO.7 when configured in clock output mode.
    BOOL    opened;

    //Если направление GPIO - выход, то включаем бит по маске
    if (GPIO_0)     direction |= HID_SMBUS_MASK_GPIO_0;
    if (GPIO_1)     direction |= HID_SMBUS_MASK_GPIO_1;
    if (GPIO_2)     direction |= HID_SMBUS_MASK_GPIO_2;
    if (GPIO_3)     direction |= HID_SMBUS_MASK_GPIO_3;
    if (GPIO_4)     direction |= HID_SMBUS_MASK_GPIO_4;
    if (GPIO_5)     direction |= HID_SMBUS_MASK_GPIO_5;
    if (GPIO_6)     direction |= HID_SMBUS_MASK_GPIO_6;
    if (GPIO_7)     direction |= HID_SMBUS_MASK_GPIO_7;

    //Если режим GPIO - push-pull, то включаем бит по маске
    if (GPIO_0_MODE)	mode |= HID_SMBUS_MASK_GPIO_0;
    if (GPIO_1_MODE)	mode |= HID_SMBUS_MASK_GPIO_1;
    if (GPIO_2_MODE)	mode |= HID_SMBUS_MASK_GPIO_2;
    if (GPIO_3_MODE)	mode |= HID_SMBUS_MASK_GPIO_3;
    if (GPIO_4_MODE)	mode |= HID_SMBUS_MASK_GPIO_4;
    if (GPIO_5_MODE)	mode |= HID_SMBUS_MASK_GPIO_5;
    if (GPIO_6_MODE)	mode |= HID_SMBUS_MASK_GPIO_6;
    if (GPIO_7_MODE)	mode |= HID_SMBUS_MASK_GPIO_7;

    //Если используется специальный режим GPIO, то включаем бит по маске
    //В расчет берутся только GPIO_0, GPIO_1, GPIO_7.
    //Для остальных GPIO специальный режим отсутствует
    if (TX)	function |= HID_SMBUS_MASK_FUNCTION_GPIO_0_TXT;
    if (RX)	function |= HID_SMBUS_MASK_FUNCTION_GPIO_1_RXT;

    if (HidSmbus_IsOpened(device, &opened) == HID_SMBUS_SUCCESS && opened)
    {
        HID_SMBUS_STATUS status = HidSmbus_SetGpioConfig(device, direction, mode, function, m_clkDiv);
        if (status == HID_SMBUS_SUCCESS)
        {
            return TRUE;
        }
            return FALSE;
    }

    return FALSE;
}
