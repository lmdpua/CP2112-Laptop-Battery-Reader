#include "BatteryCommands.h"
#include <string.h> //Без этого не работает memcpy


UINT16 ReadWord(HID_SMBUS_DEVICE device, INT *data, INT battery_register)
{
    BOOL                opened=1;               //Открылось ли устройство
    HID_SMBUS_S0        status0;                //Расширенный статус
    HID_SMBUS_S1        status1;                //Расширенный статус
    WORD                numRetries;             //Количество повторов чтения
    WORD                bytesRead;              //Количество прочитанных байт
    BYTE                numBytesToRead = 2;     //Количество байт которые нужно прочитать
    BYTE                numBytesRead = 0;       //Количество прочитанных байт
    WORD                targetAddressSize = 1;  //Количество байт в адресе
    BYTE                buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE]={0}; //Приемный буфер
    BYTE                targetAddress[16]={0};  //Адрес для чтения

    targetAddress[0] = battery_register;

    HidSmbus_IsOpened(device, &opened);
    if(opened)
    {
        if (HidSmbus_AddressReadRequest(device, BATTERY_ADDRESS << 1, numBytesToRead, targetAddressSize, targetAddress)!=HID_SMBUS_SUCCESS)return FALSE;
        if (HidSmbus_TransferStatusRequest(device)!=HID_SMBUS_SUCCESS)return FALSE;
        if (HidSmbus_GetTransferStatusResponse(device, &status0, &status1, &numRetries, &bytesRead)!=HID_SMBUS_SUCCESS)return FALSE;
        if (status1 == HID_SMBUS_S1_BUSY_ADDRESS_NACKED)
        {
            HidSmbus_Close(device);
            return FALSE;
        }
        if (HidSmbus_ForceReadResponse(device, numBytesToRead)!=HID_SMBUS_SUCCESS)return FALSE;
        if (HidSmbus_GetReadResponse(device, &status0, buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead)!=HID_SMBUS_SUCCESS)return FALSE;
        *data = (buffer[1] << 8) | buffer[0];
        return TRUE;
    }
    return FALSE;
}

UINT16 ReadBlock(HID_SMBUS_DEVICE device, char *block, INT *block_length, INT battery_register)
{
    BOOL                opened;                 //Открылось ли устройство
    HID_SMBUS_S0        status0;                //Расширенный статус 0
    HID_SMBUS_S1        status1;                //Расширенный статус 1
    WORD                numRetries;             //Количество повторов чтения
    WORD                bytesRead;              //Количество прочитанных байт HidSmbus_GetTransferStatusResponse
    BYTE                numBytesRead;           //Количество прочитанных байт HidSmbus_GetReadResponse
    BYTE                numBytesToRead=MAX_SMBUS_BLOCK_SIZE;      //Сколько байт нужно прочитать
    BYTE                totalNumBytesRead = 0;  //Сколько всего байт прочитано
    WORD                targetAddressSize = 1;  //Количество байт в адресе
    BYTE                buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE]={0}; //Приемный буфер
    BYTE                temp [MAX_SMBUS_BLOCK_SIZE]={0};          //Временный массив для сдвига
    BYTE                targetAddress[16]={0};  //Адрес откуда читать

    targetAddress[0] = battery_register;        //Передаем адрес в массив

    HidSmbus_IsOpened(device, &opened);         //Открываем устройство
    if(opened)                                  //Если открыто
    {
        if (HidSmbus_AddressReadRequest(device, BATTERY_ADDRESS << 1,
                                        numBytesToRead, targetAddressSize,
                                        targetAddress)!=HID_SMBUS_SUCCESS)return FALSE;

        if (HidSmbus_TransferStatusRequest(device)!=HID_SMBUS_SUCCESS)return FALSE;

        if (HidSmbus_GetTransferStatusResponse(device, &status0,
                                               &status1, &numRetries,
                                               &bytesRead)!=HID_SMBUS_SUCCESS)return FALSE;

        if (status1 == HID_SMBUS_S1_BUSY_ADDRESS_NACKED)
        {
            HidSmbus_Close(device);
            return FALSE;
        }

        if (HidSmbus_ForceReadResponse(device, numBytesToRead)!=HID_SMBUS_SUCCESS)return FALSE;

            do
            {
                if (HidSmbus_GetReadResponse(device, &status0,
                                             buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE,
                                             &numBytesRead)!= HID_SMBUS_SUCCESS) return FALSE;

                memcpy(&temp[totalNumBytesRead], buffer, numBytesRead); //Копируем часть прочитанных данных во временный
                                                                        //буфер, начиная с адреса totalNumBytesRead
                totalNumBytesRead += numBytesRead;                      //Увеличиваем адрес на величину прочитанных байт
            } while (totalNumBytesRead < numBytesToRead);               //Все ли мы прочитали

        INT length = temp[0];           //Нулевая ячейка временного буффера содержит длину строки
        if (length > MAX_SMBUS_BLOCK_SIZE) length = MAX_SMBUS_BLOCK_SIZE;   //Если поймали глюк и длина слишком большая, ограничим
        memcpy(&block[0], &temp[1], length); //Сдвинем весь массив на одну ячейку влево (уберем нулевую ячейку) 1й способ
//        for(BYTE i=0; i<length; i++)    //Сдвинем весь массив на одну ячейку влево (уберем нулевую ячейку) 2й способ
//        {
//            block[i] = temp[i+1];
//        }

        block [length]='\0';            //Устанавливаем признак конца строки
        *block_length = length;         //Вернем длину строки

        return TRUE;
    }

    return FALSE;
}

UINT16 ReadDataBlock(HID_SMBUS_DEVICE device, BYTE *block, INT *block_length, INT battery_register)
{
    BOOL                opened;                 //Открылось ли устройство
    HID_SMBUS_S0        status0;                //Расширенный статус 0
    HID_SMBUS_S1        status1;                //Расширенный статус 1
    WORD                numRetries;             //Количество повторов чтения
    WORD                bytesRead;              //Количество прочитанных байт HidSmbus_GetTransferStatusResponse
    BYTE                numBytesRead;           //Количество прочитанных байт HidSmbus_GetReadResponse
    BYTE                numBytesToRead=MAX_SMBUS_BLOCK_SIZE;      //Сколько байт нужно прочитать
    BYTE                totalNumBytesRead = 0;  //Сколько всего байт прочитано
    WORD                targetAddressSize = 1;  //Количество байт в адресе
    BYTE                buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE]={0}; //Приемный буфер
    BYTE                temp [MAX_SMBUS_BLOCK_SIZE]={0};          //Временный массив для сдвига
    BYTE                targetAddress[16]={0};  //Адрес откуда читать

    targetAddress[0] = battery_register;        //Передаем адрес в массив

    HidSmbus_IsOpened(device, &opened);         //Открываем устройство
    if(opened)                                  //Если открыто
    {
        if (HidSmbus_AddressReadRequest(device, BATTERY_ADDRESS << 1,
                                        numBytesToRead, targetAddressSize,
                                        targetAddress)!=HID_SMBUS_SUCCESS)return FALSE;

        if (HidSmbus_TransferStatusRequest(device)!=HID_SMBUS_SUCCESS)return FALSE;

        if (HidSmbus_GetTransferStatusResponse(device, &status0,
                                               &status1, &numRetries,
                                               &bytesRead)!=HID_SMBUS_SUCCESS)return FALSE;

        if (status1 == HID_SMBUS_S1_BUSY_ADDRESS_NACKED)
        {
            HidSmbus_Close(device);
            return FALSE;
        }

        if (HidSmbus_ForceReadResponse(device, numBytesToRead)!=HID_SMBUS_SUCCESS)return FALSE;
            do
            {
                if (HidSmbus_GetReadResponse(device, &status0,
                                             buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE,
                                             &numBytesRead)!= HID_SMBUS_SUCCESS) return FALSE;

                memcpy(&temp[totalNumBytesRead], buffer, numBytesRead); //Копируем часть прочитанных данных во временный
                                                                        //буфер, начиная с адреса totalNumBytesRead
                totalNumBytesRead += numBytesRead;                      //Увеличиваем адрес на величину прочитанных байт
            } while (totalNumBytesRead < numBytesToRead);               //Все ли мы прочитали

        INT length = temp[0];           //Нулевая ячейка временного буффера содержит длину строки
        if (length > MAX_SMBUS_BLOCK_SIZE) length = MAX_SMBUS_BLOCK_SIZE;   //Если поймали глюк и длина слишком большая, ограничим
        memcpy(&block[0], &temp[1], length); //Сдвинем весь массив на одну ячейку влево (уберем нулевую ячейку) 1й способ
//        for(BYTE i=0; i<length; i++)    //Сдвинем весь массив на одну ячейку влево (уберем нулевую ячейку) 2й способ
//        {
//            block[i] = temp[i+1];
//        }
//      Тут это не понадобится, так как на выходе будет не String, а просто массив
//        block [length]='\0';            //Устанавливаем признак конца строки
        *block_length = length;         //Вернем длину массива

        return TRUE;
    }

    return FALSE;
}

//LENOVO?
//BOOL Lenovo(HID_SMBUS_DEVICE device)
//{
//    BOOL                opened;                 //Открылось ли устройство
//    BYTE                targetAddress[16]={0};  //Адрес откуда читать

//    targetAddress[0] = 0x3c;        //Передаем адрес в массив// CELL4_VOLTAGE

//    HidSmbus_IsOpened(device, &opened);         //Открываем устройство
//    if(opened)                                  //Если открыто
//    {
////	if (!HAL_I2C_IsDeviceReady(&hi2c2, I2C_ADDRESS << 1, 1, I2C_TIMEOUT))
////	{
//        BYTE length_read = I2c_ReadBlock(CELL4_VOLTAGE);
//        for(uint8_t i=0; i<length_read; i++)
//        {
//            if(memcmp(&i2cBuffer[i],"LENOVO" ,6)==0){return 1;}else
//            if(memcmp(&i2cBuffer[i],"Lenovo" ,6)==0){return 1;}else
//            if(memcmp(&i2cBuffer[i],"IBM" ,3)==0){return 1;}
//        }
//        return 0;
//    }
//    return 0;
//}
