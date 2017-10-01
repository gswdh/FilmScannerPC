#include "ftdi.h"

ftdi::ftdi()
{



}

int ftdi::ListDevices(FTDIDevices *devices)
{
    // pointer to array of 3 pointers
    char *BufPtrs[5];

    // initialize the array of pointers
    BufPtrs[0] = devices->device1;
    BufPtrs[1] = devices->device2;
    BufPtrs[2] = devices->device3;
    BufPtrs[3] = devices->device4;
    BufPtrs[4] = NULL; // last entry should be NULL

    // Number of
    DWORD numDevs;

    // Get the list
    ftStatus = FT_ListDevices(BufPtrs, &numDevs, FT_LIST_ALL|FT_OPEN_BY_SERIAL_NUMBER);

     devices->ndevs = numDevs;

    // Status message
    if(ftStatus == FT_OK) return 0;
    else return 1;
}

int ftdi::Connect(char serial[])
{
    // Open device by serial number
    ftStatus = FT_OpenEx(serial, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

    // Reset the FTDI device
    ftStatus = FT_SetBitMode(ftHandle, Mask, 0);

    // Delay for the device to restart
    Sleep(100);

    // Set the mode to 245 sync
    ftStatus = FT_SetBitMode(ftHandle, Mask, MODE_SYNC_245);

    // Configure latency
    ftStatus = FT_SetLatencyTimer(ftHandle, LatencyTimer);

    // Configure USB params
    ftStatus = FT_SetUSBParameters(ftHandle, 32768, 0);

    // Set the flow control
    ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0, 0);

    // Status message
    if(ftStatus == FT_OK) return 0;
    else return 1;
}

int ftdi::Disconnect()
{
    // Close the device
    ftStatus = FT_Close(ftHandle);

    // Status message
    if(ftStatus == FT_OK) return 0;
    else return 1;
}

int ftdi::Write(FTDIData data)
{
    // Check to see if we're connected to a device
    if(ftHandle == NULL) return 3;

    // Number of bytes sent
    DWORD nbytes_tx = 0;

    // Write the FTDI device
    ftStatus = FT_Write(ftHandle, data.buffer, data.buff_len, &nbytes_tx);

    // Status message
    if(nbytes_tx != data.buff_len) return 2;

    // Status message
    if(ftStatus == FT_OK) return 0;
    else return 1;
}



int ftdi::Read(FTDIData *data)
{
    // Check to see if we're connected to a device
    if(ftHandle == NULL) return 2;

    // Attempt to read the maximum amount of data
    //ftStatus = FT_Read(ftHandle, data->buffer, 65535, &data->buff_len);

    // Status message
    if(ftStatus == FT_OK) return 0;
    else return 1;
}
