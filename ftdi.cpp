#include "ftdi.h"

ftdi::ftdi()
{

}

int ftdi::GetDevices()
{
    // Variable for the number of devices connected
    DWORD numDevs;

    // Just get the number of devices attached
    ftStatus = FT_ListDevices(&numDevs, NULL,FT_LIST_NUMBER_ONLY);

    // Return error if not ok
    if(ftStatus != FT_OK)return 1;

    //
    //int nDevices = int(numDevs);

    // Create memory for the device names
    //char Buffer1[64]; // buffer for description of first device
    //char Buffer2[64]; // buffer for description of second device


    //ftStatus = FT_ListDevices((PVOID)devIndex,Buffer,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);

    // Return error code
    if(ftStatus == FT_OK)return OK;
    else return 1;
}

int ftdi::ConnectToDevice(char serial_number[])
{
    // Attempt to open with the serial number
    ftStatus = FT_OpenEx(serial_number, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

    // Set the serial number
    scanner_params.serial_number = serial_number;
    scanner_params.bit_depth = 0;
    scanner_params.gain = 0;
    scanner_params.line_length = 0;
    scanner_params.n_lines = 0;
    scanner_params.offset = 0;
    scanner_params.scanner_running = FALSE;

    // Return status
    if (ftStatus == FT_OK)return OK;
    else return ERROR;
}

void ftdi::DisconnectFromDevice(void)
{
    // Close the FTDI connection
    FT_Close(&ftHandle);

    // Set the connected device to null
    scanner_params.serial_number = "";
}

int ftdi::GetLinesAvalible()
{
    // RxBytes variable
    DWORD RxBytes;

    // Lines Variable
    int n_lines = 0;

    // Get the number of bytes avalible
    ftStatus = FT_GetQueueStatus(ftHandle, &RxBytes);

    // Check to see if OK
    if(ftStatus != FT_OK)
    {
        // Return with 0 lines
        return n_lines;
    }

    // Else all good
    else
    {
        if(scanner_params.bit_depth == BIT_DEPTH_8)
        {
            // Set the number of lines availible
            n_lines = int(RxBytes) / scanner_params.line_length;
        }

        else if(scanner_params.bit_depth == BIT_DEPTH_16)
        {
            // Set the number of lines availible
            n_lines = int(RxBytes) / scanner_params.line_length / 2;
        }

        // Return
        return n_lines;
    }
}

SLine ftdi::GetLine(unsigned int timeout)
{
    // RxBytes variable
    DWORD RxBytes;

    // New SLine object
    SLine this_line;

    // Set the line length
    this_line.length = scanner_params.line_length;

    // Get the bit depth
    if(scanner_params.bit_depth == BIT_DEPTH_8)RxBytes = scanner_params.bit_depth;
    else if(scanner_params.bit_depth == BIT_DEPTH_16)RxBytes = 2 * scanner_params.bit_depth;

    // Else error
    else
    {
        // Set parameters to zero for an error
        this_line.length = 0;
        this_line.line_sum = 0;

        // Return
        return this_line;
    }

    // To make sure we get all the data
    DWORD BytesReceived = 0;

    // Create a timeout varible
    unsigned int timeout_cntr = 0;

    // Bytes availble
    DWORD RxBytesAval = 0;

    // Make sure the data is there
    while((timeout_cntr < timeout) && (RxBytesAval < RxBytes)){

        // Get the number of bytes avalible
        ftStatus = FT_GetQueueStatus(ftHandle, &RxBytesAval);


        // Check to see if OK
        if(ftStatus != FT_OK)
        {
            // Set parameters to zero for an error
            this_line.length = 0;
            this_line.line_sum = 0;

            // Return
            return this_line;
        }
    }

    // Create the buffer
    char RxBuffer[RxBytes];

    // Retrieve the data
    ftStatus = FT_Read(ftHandle, RxBuffer, RxBytes, &BytesReceived);

    // Make sure the read went okay
    if((ftStatus != FT_OK) || (RxBytes != BytesReceived))
    {
        // Set parameters to zero for an error
        this_line.length = 0;
        this_line.line_sum = 0;

        // Return
        return this_line;
    }

    // Zero the sum
    this_line.line_sum = 0;

    // 8 bits of depth
    if(scanner_params.bit_depth == BIT_DEPTH_8)
    {
        // Write the data to the pointer
        for(int i = 0; i < this_line.length; i++)
        {
            // Transfer the line TODO: correct for the int cast
            this_line.data[i] = int(RxBuffer[i]) + 255;

            // Get the sum of the line
            this_line.line_sum += this_line.data[i];
        }
    }

    // 16 bits
    else if(scanner_params.bit_depth == BIT_DEPTH_16)
    {
        // Write the data to the pointer
        for(int i = 0; i < (this_line.length * 2); i = i + 2)
        {
            // TODO: correct for the int cast
            this_line.data[i] = int((RxBuffer[i] << 8) + RxBuffer[i + 1]);

            // Get the sum of the line
            this_line.line_sum += this_line.data[i];
        }
    }

    // Return
    return this_line;
}

int ftdi::StartScan(unsigned int frames, unsigned int dpi, unsigned int bit_depth, unsigned int gain, unsigned int offset)
{
    // Set the program variables
    scanner_params.bit_depth = bit_depth;
    scanner_params.gain = gain;
    scanner_params.line_length = dpi;
    scanner_params.n_lines = frames * dpi * 1.5 * 1.1;
    scanner_params.offset = offset;
    scanner_params.scanner_running = TRUE;

    // Set the bit depth
    this->SendCommand(SCANNER_BIT_DEPTH);
    if(scanner_params.bit_depth == BIT_DEPTH_8)this->SendParam(SCANNER_8BIT);
    else this->SendParam(SCANNER_16BIT);

    // Set the line length
    this->SendCommand(SCANNER_LINE_LENGTH);
    if(scanner_params.line_length == SCANNER_2K_LENGTH)this->SendParam(SCANNER_2K_LENGTH);
    else this->SendParam(SCANNER_512_LENGTH);

    // Set the number of lines
    this->SendCommand(SCANNER_N_LINES);
    this->SendParamEX(scanner_params.n_lines);

    // Set the gain
    this->SendCommand(SCANNER_GAIN);
    this->SendParamEX(scanner_params.gain);

    // Set the offest
    this->SendCommand(SCANNER_OFFSET);
    this->SendParamEX(scanner_params.offset);

    // Now the scanner has been configured, start the scan
    this->SendCommand(SCANNER_RUN);
    this->SendParam(SCANNER_START);

    // This for now TODO: implement error checking
    return 0;
}

int ftdi::SendCommand(byte command)
{
    // Write the command to the buffer making sure the MSB is set for a command
    char TxBuffer[1] = {0x80 || command};

    // Send variables
    DWORD TxBytes = 1, TxBytesSent;

    // Send the settings to the scanner
    ftStatus = FT_Write(ftHandle, TxBuffer, TxBytes, &TxBytesSent);

    // Make sure it sent
    if((ftStatus != FT_OK) || (TxBytes != TxBytesSent))return 0;
    else return 1;
}

int ftdi::SendParam(byte param)
{
    // Write the param to the buffer making sure the MSB is not set for a a parameter
    char TxBuffer[1] = {0x7F && param};

    // Send variables
    DWORD TxBytes = 1, TxBytesSent;

    // Send the settings to the scanner
    ftStatus = FT_Write(ftHandle, TxBuffer, TxBytes, &TxBytesSent);

    // Make sure it sent
    if((ftStatus != FT_OK) || (TxBytes != TxBytesSent))return 0;
    else return 1;
}

int ftdi::SendParamEX(unsigned int param)
{
    // Write the param to the buffer making sure the MSB is not set for a a parameter
    char TxBuffer[3] = {0x7F && (param >> 14), 0x7F && (param >> 7), 0x7F && param};

    // Send variables
    DWORD TxBytes = 3, TxBytesSent;

    // Send the settings to the scanner
    ftStatus = FT_Write(ftHandle, TxBuffer, TxBytes, &TxBytesSent);

    // Make sure it sent
    if((ftStatus != FT_OK) || (TxBytes != TxBytesSent))return 0;
    else return 1;
}


