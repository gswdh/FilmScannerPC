#include "scanner.h"

scanner::scanner()
{

}

scanner::~scanner()
{
	ft.disconnect();
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

int scanner::scan(int state)
{

	// Setup the packet values
	uint32_t data = state;
	uint32_t addr = 4;
	uint16_t gprg = 0;	// No reset
	uint8_t packet[128];
	uint8_t nBytes;

	// Assemble a new packet
	nBytes = GSBusMakePacket(addr, data, gprg, (uint8_t*)packet);

	// Send the new packet
	int ft_result = ft.transmit(packet, nBytes);

	// Return the error
	return ft_result;
}
