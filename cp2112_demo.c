// This code is from Nick Dademo https://github.com/nickdademo/cp2112_demo with minor modifications
#include <stdio.h>
#include <stdlib.h>

#include "smbus.h"

/*
NOTE: As command response lengths may differ between gas gauges, ensure
sbsCommandResponseLength contains the correct lengths for your
particular device (check datasheet).

For example, some typical variations:
Manufacturer Name [0x20] = 20+1 bytes / 11+1 bytes
Device Name [0x21] = 20+1 bytes / 7+1 bytes
*/
const WORD sbsCommandResponseLength[] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   // 0x00 - 0x09
    2, 2, 1, 1, 1, 2, 2, 2, 2, 2,   // 0x0A - 0x13
    2, 2, 2, 2, 2, 2, 2, 2, 2, 0,   // 0x14 - 0x1D
    0, 0, 21, 21, 5, 15             // 0x1E - 0x23
};

enum sbsCommands {
    MANUFACTURER_ACCESS,        // 0x00
    REMAINING_CAPACITY_ALARM,   // 0x01
    REMAINING_TIME_ALARM,       // 0x02
    BATTERY_MODE,               // 0x03
    AT_RATE,                    // 0x04
    AT_RATE_TIME_TO_FULL,       // 0x05
    AT_RATE_TIME_TO_EMPTY,      // 0x06
    AT_RATE_OK,                 // 0x07
    TEMPERATURE,                // 0x08
    VOLTAGE,                    // 0x09
    CURRENT,                    // 0x0A
    AVERAGE_CURRENT,            // 0x0B
    MAX_ERROR,                  // 0x0C
    RELATIVE_STATE_OF_CHARGE,   // 0x0D
    ABSOLUTE_STATE_OF_CHARGE,   // 0x0E
    REMAINING_CAPACITY,         // 0x0F
    FULL_CHARGE_CAPACITY,       // 0x10
    RUN_TIME_TO_EMPTY,          // 0x11
    AVERAGE_TIME_TO_EMPTY,      // 0x12
    AVERAGE_TIME_TO_FULL,       // 0x13
    CHARGING_CURRENT,           // 0x14
    CHARGING_VOLTAGE,           // 0x15
    BATTERY_STATUS,             // 0x16
    CYCLE_COUNT,                // 0x17
    DESIGN_CAPACITY,            // 0x18
    DESIGN_VOLTAGE,             // 0x19
    SPECIFICATION_INFO,         // 0x1A
    MANUFACTURER_DATE,          // 0x1B
    SERIAL_NUMBER,              // 0x1C
    RESERVED1,                  // 0x1D
    RESERVED2,                  // 0x1E
    RESERVED3,                  // 0x1F
    MANUFACTURER_NAME,          // 0x20
    DEVICE_NAME,                // 0x21
    DEVICE_CHEMISTRY,           // 0x22
    MANUFACTURER_DATA           // 0x23
};

#define BITRATE_HZ                  25000
#define ACK_ADDRESS                 0x02
#define AUTO_RESPOND                FALSE
#define WRITE_TIMEOUT_MS            1000
#define READ_TIMEOUT_MS             1000
#define TRANSFER_RETRIES            0
#define SCL_LOW_TIMEOUT             TRUE
#define RESPONSE_TIMEOUT_MS         1000

#define CHARGER_SLAVE_ADDRESS_W     0x12
#define BATTERY_SLAVE_ADDRESS_W     0x16

int mainn(int argc, char* argv[])
{
    HID_SMBUS_DEVICE    m_hidSmbus;
    BYTE                buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];
    BYTE                targetAddress[16];

    // Open device
    if(SMBus_Open(&m_hidSmbus) != 0)
    {
        fprintf(stderr,"ERROR: Could not open device.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
//    fprintf(stderr,"Device successfully opened.\r\n");

    // Configure device
    if(SMBus_Configure(m_hidSmbus, BITRATE_HZ, ACK_ADDRESS, AUTO_RESPOND, WRITE_TIMEOUT_MS, READ_TIMEOUT_MS, SCL_LOW_TIMEOUT, TRANSFER_RETRIES, RESPONSE_TIMEOUT_MS) != 0)
    {
        fprintf(stderr,"ERROR: Could not configure device.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
//    fprintf(stderr,"Device successfully configured.\r\n");
    fprintf(stderr, "***LiPo Battery status registers***\r\n");
    // Voltage [0x09]
    targetAddress[0] = VOLTAGE;
    if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[VOLTAGE], 1, targetAddress) != sbsCommandResponseLength[VOLTAGE])
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    UINT16 voltage_mV = (buffer[1] << 8) | buffer[0];
    fprintf(stderr, "Voltage = %d mV\r\n", voltage_mV);

    // Current [0x0A]
    targetAddress[0] = CURRENT;
    if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[CURRENT], 1, targetAddress) != sbsCommandResponseLength[CURRENT])
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    INT16 current_mA = (buffer[1] << 8) | buffer[0];
    fprintf(stderr, "Current = %d mA\r\n", current_mA);

    // Relative State of Charge [0x0D]
    targetAddress[0] = RELATIVE_STATE_OF_CHARGE;
    if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[RELATIVE_STATE_OF_CHARGE], 1, targetAddress) != sbsCommandResponseLength[RELATIVE_STATE_OF_CHARGE])
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    BYTE rsoc = buffer[0];
    fprintf(stderr, "State of Charge = %d %%\r\n", rsoc);

    // Remaining Capacity [0x0F]
    targetAddress[0] = REMAINING_CAPACITY;
    if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[REMAINING_CAPACITY], 1, targetAddress) != sbsCommandResponseLength[REMAINING_CAPACITY])
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    UINT16 remCap = (buffer[1] << 8) | buffer[0];
    fprintf(stderr, "Remaining Capacity = %d mAh\r\n", remCap);
// Check if discharging
    if (current_mA <= 0)
    {
        // Average Time to Empty [0x12]
        targetAddress[0] = AVERAGE_TIME_TO_EMPTY;
        if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[AVERAGE_TIME_TO_EMPTY], 1, targetAddress) != sbsCommandResponseLength[AVERAGE_TIME_TO_EMPTY])
        {
            fprintf(stderr, "ERROR: Could not perform SMBus read.\r\n");
            SMBus_Close(m_hidSmbus);
            return -1;
        }
        UINT16 avgTimeToEmpty = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Average Time to Empty = %d minutes\r\n", avgTimeToEmpty);
    }
    else
    {
        // Average Time to Full [0x13]
        targetAddress[0] = AVERAGE_TIME_TO_FULL;
        if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[AVERAGE_TIME_TO_FULL], 1, targetAddress) != sbsCommandResponseLength[AVERAGE_TIME_TO_FULL])
        {
            fprintf(stderr, "ERROR: Could not perform SMBus read.\r\n");
            SMBus_Close(m_hidSmbus);
            return -1;
        }
        UINT16 avgTimeToFull = (buffer[1] << 8) | buffer[0];
        fprintf(stderr, "Average Time to Full = %d minutes\r\n", avgTimeToFull);
    }
    // Temperature [0x08]
    targetAddress[0] = TEMPERATURE;
    if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[TEMPERATURE], 1, targetAddress) != sbsCommandResponseLength[TEMPERATURE])
    {
        fprintf(stderr, "ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    UINT16 TempC = (buffer[1] << 8) | buffer[0];
    TempC = (TempC / 10) - 273;
    fprintf(stderr, "Temperature = %d C\r\n", TempC);

    /*
    // Manufacturer Name [0x20]
    targetAddress[0] = MANUFACTURER_NAME;
    if (SMBus_Read(m_hidSmbus, buffer, BATTERY_SLAVE_ADDRESS_W, sbsCommandResponseLength[MANUFACTURER_NAME], 1, targetAddress) < 1)
    {
        fprintf(stderr,"ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    fprintf(stderr, "Manufacturer Name = ");
    // NOTE: Length of string is stored in first received byte
    for(int i = 1; i < buffer[0] + 1; i++)
    {
        fprintf(stderr, "%c", buffer[i]);
    }
    fprintf(stderr, "\r\n");

    // Check if charger is present
    // Charger Status [0x13]
    targetAddress[0] = 0x13;
    if (SMBus_Read(m_hidSmbus, buffer, CHARGER_SLAVE_ADDRESS_W, 2, 1, targetAddress) != 2)
    {
        fprintf(stderr, "ERROR: Could not perform SMBus read.\r\n");
        SMBus_Close(m_hidSmbus);
        return -1;
    }
    UINT16 chargerStatus = (buffer[1] << 8) | buffer[0];
    if (chargerStatus & 0x8000)
    {
        fprintf(stderr, "Charger connected.\r\n");
    }
    else
    {
        fprintf(stderr, "Charger NOT connected.\r\n");
    }
*/
    // Success
//    fprintf(stderr, "Done! Exiting...\r\n");
    SMBus_Close(m_hidSmbus);
    system("pause"); // wait for Enter keypress to keep the terminal window from going away
    return 0;
}
