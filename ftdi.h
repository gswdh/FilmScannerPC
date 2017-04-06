#ifndef FTDI_H
#define FTDI_H

#include "ftd2xx.h"
#include <QString>
#include <string>

#define OK 0
#define BIT_DEPTH_8 255
#define BIT_DEPTH_16 65535

// Command defines for the scanner operation
#define SCANNER_RUN 0x00
#define SCANNER_BIT_DEPTH 0x01
#define SCANNER_LINE_LENGTH 0x02
#define SCANNER_N_LINES 0x03
#define SCANNER_GAIN 0x04
#define SCANNER_OFFSET 0x05

// Parameters for the registers
// Scanner run
#define SCANNER_START 0x01
#define SCANNER_STOP 0x00

// Bit depth
#define SCANNER_8BIT 0x00
#define SCANNER_16BIT 0x01

// Line Length
#define SCANNER_512_LENGTH 0x00
#define SCANNER_2K_LENGTH 0x01

/*
 *  A type called SLine, a type returned from the GetLine function.
 *
 *  length, number of elements in the line
 *  data, element values of the line
 *  line_sum, the all the data in the line summed
 */
typedef struct {
    unsigned short int length;
    unsigned short int data[16384];
    unsigned long line_sum;
} SLine;

/*
 *  A type called SParams, contains the current settings for the scanner
 *
 *  serial_number, serial of connected device
 *  line_length, current setting of the line length in elements
 *  bit_depth, current bit depth setting of the scanner, 8 or 16bit
 *  offset, control of the DC offset inside the scanner
 *  gain, control of the ADC gain in the scanner
 *  scanner_running, true or false of whether a scan is in progress
 */
typedef struct {
    QString serial_number;
    unsigned short int line_length;
    unsigned short int bit_depth;
    unsigned short int offset;
    unsigned short int gain;
    unsigned int n_lines;
    bool scanner_running;
} SParams;

class ftdi
{

private:
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    SParams scanner_params;
    int SendCommand(byte command);
    int SendParam(byte param);
    int SendParamEX(unsigned int param);

public:
    ftdi();
    int GetDevices(void);
    int ConnectToDevice(char serial_number[]);
    void DisconnectFromDevice(void);
    int GetLinesAvalible();
    SLine GetLine(unsigned int timeout);
    int StartScan(unsigned int frames, unsigned int dpi, unsigned int bit_depth, unsigned int gain, unsigned int offset);
};

#endif // FTDI_H
