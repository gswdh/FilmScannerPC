#include "FTDIWrapper/inc/ftdi.h"
#include "GSBus/software/gsbus.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <vector>
#include <functional>

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
	uint8_t scan_loop = 0;

public:
	scanner();
	int8_t connect(std::string serial_n);
	void scanStart(std::function<void(std::vector<uint8_t>* pData)> lineCallBack);
	void scanStop(void);
	int8_t setReg(uint32_t addr, uint32_t data);
	int8_t setBlackLevel(uint32_t level);
	int8_t setGain(double gain);
	int8_t setScanSampleMode(uint32_t mode);
	int8_t setScanFrameRate(uint32_t rate);
	int8_t setScanEnable(uint32_t state);
	int8_t setLEDBrightness(double level);
	int8_t setMotorSpeed(uint32_t speed, uint32_t dir);
	int8_t setMotorEnable(uint32_t enable);
	int8_t init(std::string serial_n);
	int8_t getData(std::vector<unsigned char>* pData, uint32_t nBytes);
	int8_t getQueue(uint32_t * nBytes);
	void dataProcessor(std::function<void(std::vector<uint8_t>* pData)> lineCallBack);
	~scanner();
};



