#include "scanner.h"

scanner::scanner()
{

}

scanner::~scanner()
{
	ft.disconnect();
}

int8_t scanner::init(std::string serial_n)
{
	int scan_result = -1;

	// Connect to the scanner
	scan_result = this->connect(serial_n);

	// Setup the black level
	scan_result = this->setBlackLevel(38275);

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

int8_t scanner::connect(std::string serial_n)
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

int8_t scanner::setReg(uint32_t addr, uint32_t data)
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
    return 0;
}

int8_t scanner::setScanSampleMode(uint32_t mode)
{
	return this->setReg(SCAN_SUB_SAMPLE_ADDR, mode);
}

int8_t scanner::setScanFrameRate(uint32_t rate)
{
	return this->setReg(SCAN_FRAME_RATE_ADDR, rate);
}

int8_t scanner::setScanEnable(uint32_t state)
{
	return this->setReg(SCAN_EN_ADDR, state);
}

int8_t scanner::setLEDBrightness(double level)
{
	// Convert from a float
	uint32_t pwm_level = 255 * level;

	// Make a cap
	if(pwm_level > 255) pwm_level = 255;

	return this->setReg(LED_PWM_ADDR, pwm_level);
}

int8_t scanner::setMotorSpeed(uint32_t speed, uint32_t dir)
{
	int scan_result = -1;

	scan_result = this->setReg(MTR_SPEED_ADDR, speed);
	scan_result = this->setReg(MTR_DIR_ADDR, dir);

	return scan_result;
}

int8_t scanner::setMotorEnable(uint32_t enable)
{
	return this->setReg(MTR_EN_ADDR, enable);
}

int8_t scanner::setBlackLevel(uint32_t level)
{
	return this->setReg(DAC_OFFSET_ADDR, level);
}

int8_t scanner::setGain(double gain)
{
	// Convert the gain for something for a reg
	uint32_t gain_level = 65535 / gain;

	// Make a cap
	if(gain_level > 65535) gain_level = 65535;

	// Set the reg
	return this->setReg(DAC_GAIN_ADDR, gain_level);
}

int8_t scanner::getData(uint8_t * pData, uint32_t nBytes)
{
	int ft_result = -1;

	// Now get the data
    ft_result = ft.receive(pData, nBytes);

	// Result
	return ft_result;
}

int8_t scanner::getQueue(uint32_t * nBytes)
{
	return ft.getQueLen(nBytes);
}

void scanner::scanStart(void)
{
	// Set the loop to be enabled
	this->scan_loop = 1;

	// Start the scanning in the scanner
	this->setScanEnable(1);
}

void scanner::scanStop(void)
{
	// Simply stop the loop and it'll quit
    this->scan_loop = 0;

    // Stop the scanning in the scanner
    this->setScanEnable(0);
}

int8_t scanner::getLine(uint8_t * line, uint32_t * line_len)
{
    uint8_t data_new[65536];

    uint32_t bytes = 0;
    int8_t result = -1;

        // Check if there's enough data, if not...
        if(this->data_len < 2047)
        {
            // Get the current number
            result = this->getQueue(&bytes);

            // Read
            result = this->getData(data_new, bytes);

            // Attach the data onto the remainder
            memcpy(this->data + data_len, data_new, bytes);
            this->data_len += bytes;
        }

        // Now we have enough data
        else
        {
            // Process the line
            result = this->LineGrabber(line);
        }

        // If there's an issue
        if(result == -1)
        {
            std::cout << "ERROR Result = " << int(result) << std::endl;
        }

        // Else all good
        return 0;
}


int8_t scanner::LineGrabber(uint8_t * line)
{
    // Go through the data pulled
    for(uint64_t i = 0; i < this->data_len; i++)
    {
        // Hit an end of line character
        if(this->data[i] == 255)
        {
            // If we have a complete (valid) line
            if(i == 2047)
            {
                // Create the line
                memcpy(line, this->data, i);

                // Remove the line elements from the raw data by shifting them
                memmove(this->data, this->data + i+1, this->data_len - (i + 1));
                this->data_len -= (i + 1);

                // Return 0 for success
                return 0;
            }

            // Return -1 for line error
            else
            {
            	// Remove the line elements from the raw data
                memmove(this->data, this->data + i+1,this->data_len - (i + 1));
                this->data_len -= (i + 1);

                return -1;
            }
        }

        // There's no end of line
        if(i == (this->data_len - 1))
        {
            // Remove the line elements from the raw data
            this->data_len = 0;

			return -1;
		}
    }

    // Return -1 for line error
    return -1;
}

