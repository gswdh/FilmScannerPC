#ifndef FTDI_H
#define FTDI_H

#include "ftd2xx.h"

// Data variable
struct FTDIData{
    char buffer[65535];
    unsigned int buff_len = 0;
};

// Devices variable
struct FTDIDevices{
    char device1[64];
    char device2[64];
    char device3[64];
    char device4[64];
    unsigned int ndevs = 0;
};

class ftdi
{
private:
    // Internal variables
    DWORD LatencyTimer = 100;
    DWORD deviceID;

    UCHAR MODE_SYNC_245 = 0x40;
    UCHAR Mask = 0xFF;

    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    FT_DEVICE ftDevice;

public:
    ftdi();
    int ListDevices(FTDIDevices *devices);
    int Connect(char serial[]);
    int Disconnect();
    int Write(FTDIData data);
    int Read(FTDIData *data);
};

#endif // FTDI_H
