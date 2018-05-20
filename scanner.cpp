#include "scanner.h"

scanner::scanner()
{

}

scanner::~scanner()
{
	ft.disconnect();
}

int scanner::init(std::string serial_n)
{
	int scan_result = -1;

	// Connect to the scanner
	scan_result = this->connect(serial_n);

	// Setup the black level
	scan_result = this->setBlackLevel(39000);

	// Set the gain to the lowest
	scan_result = this->setGain(1);

	// Set the motor speed for full speed scan
	scan_result = this->setMotorSpeed(37306, 1);

	// Set the LED value to off
	scan_result = this->setLEDBrightness(0);

	// Stop any scanning
	scan_result = this->setScanEnable(0);

	// Full resolution scan
	scan_result = this->setScanSampleMode(0);

	// Full frame rate
	scan_result = this->setScanFrameRate(0);



	// Return result
	return scan_result;
}

int scanner::connect(std::string serial_n)
{
	// Result
	int ft_result = -1;

	// Connect to the camera FTDI chip
	ft_result = ft.connect(serial_n);

	// Flush the USB FIFOs
	ft_result = ft.purge();

	// Return the success
	return ft_result;
}

int scanner::setReg(uint32_t addr, uint32_t data)
{
	// Setup the packet values
	uint16_t gprg = 0;
	uint8_t packet[128];
	uint8_t nBytes;

	// Assemble a new packet
	nBytes = GSBusMakePacket(addr, data, gprg, (uint8_t*)packet);

	// Send the new packet
	int ft_result = ft.transmit(packet, nBytes);

	// Return the error
	return ft_result;
}

int scanner::setScanSampleMode(uint32_t mode)
{
	return this->setReg(SCAN_SUB_SAMPLE_ADDR, mode);
}

int scanner::setScanFrameRate(uint32_t rate)
{
	return this->setReg(SCAN_FRAME_RATE_ADDR, rate);
}

int scanner::setScanEnable(uint32_t state)
{
	return this->setReg(SCAN_EN_ADDR, state);
}

int scanner::setLEDBrightness(double level)
{
	// Convert from a float
	uint32_t pwm_level = 255 * level;

	// Make a cap
	if(pwm_level > 255) pwm_level = 255;

	return this->setReg(LED_PWM_ADDR, pwm_level);
}

int scanner::setMotorSpeed(uint32_t speed, uint32_t dir)
{
	int scan_result = -1;

	scan_result = this->setReg(MTR_SPEED_ADDR, speed);
	scan_result = this->setReg(MTR_DIR_ADDR, dir);

	return scan_result;
}

int scanner::setMotorEnable(uint32_t enable)
{
	return this->setReg(MTR_EN_ADDR, enable);
}

int scanner::setBlackLevel(uint32_t level)
{
	return this->setReg(DAC_OFFSET_ADDR, level);
}

int scanner::setGain(double gain)
{
	// Convert the gain for something for a reg
	uint32_t gain_level = 65535 / gain;

	// Make a cap
	if(gain_level > 65535) gain_level = 65535;

	// Set the reg
	return this->setReg(DAC_GAIN_ADDR, gain_level);
}

int scanner::getData(std::vector<unsigned char>* pData, uint32_t nBytes)
{
	int ft_result = -1;

	// Now get the data
	ft_result = ft.receive(pData, nBytes);

	// Result
	return ft_result;
}

int scanner::getQueue(unsigned long int* nBytes)
{
	return ft.getQueLen(nBytes);
}
