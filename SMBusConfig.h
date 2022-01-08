#ifndef SMBUSCONFIG_H
#define SMBUSCONFIG_H

#include "types.h"

#define NUMBER_OF_SBS_ADDRESSES 37

// Режим работы шины SMBUS
#define BITRATE_HZ                  50000
#define ACK_ADDRESS                 0x02
#define AUTO_RESPOND                FALSE
#define WRITE_TIMEOUT_MS            1000
#define READ_TIMEOUT_MS             1000
#define TRANSFER_RETRIES            0
#define SCL_LOW_TIMEOUT             TRUE
#define RESPONSE_TIMEOUT_MS         1000

#define BATTERY_ADDRESS             0x0B
#define MAX_SMBUS_BLOCK_SIZE        32


#endif // SMBUSCONFIG_H
