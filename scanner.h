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

// GSBUS REGISTER VALUES
#define MTR_EN

class scanner
{
private:
	ftdi ft;

public:
	scanner();
	int connect(std::string serial_n);
	int scan(int state);
	int setReg(uint32_t addr, uint32_t data);
	int setBlackLevel(uint32_t level);
	int setGain(double gain);
	int setScanSampleMode(uint32_t mode);
	int setScanFrameRate(uint32_t rate);
	int setScanEnable(uint32_t state);
	int setLEDBrightness(double level);
	int setMotorSpeed(uint32_t speed, uint32_t dir);
	int setMotorEnable(uint32_t enable);
	int init(std::string serial_n);
	~scanner();
};



