#include "FTDIWrapper/inc/ftdi.h"
#include "GSBus/software/gsbus.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>

// Status return values
#define SCAN_OK 0
#define SCAN_ER 1

// GSBUS ADDRESSES
#define MTR_EN_ADDR 0
#define MTR_DIR_ADDR 1
#define MTR_SPEED_ADDR 2
#define LED_PWM_ADDR 3
#define SCAN_EN_ADDR 4
#define SCAN_SUB_SAMPLE_ADDR 5
#define SCAN_FRAME_RATE_ADDR 6
#define DAC_GAIN_ADDR 7
#define DAC_OFFSET_ADDR 8

class scanner
{
private:
	ftdi ft;

public:
	scanner();
	int connect(std::string serial_n);
	int scan(int state);
	~scanner();
};
