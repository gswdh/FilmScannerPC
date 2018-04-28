#include "scanner.h"



scanner::scanner()
{

}

scanner::~scanner()
{

}



int scanner::connect()
{
	std::string serial_n;

	// Attempt to connect to the scanner
	if(ft.connect(serial_n) != FTDI_OK)
		return SCAN_ER;

	// Reset the scanner TODO: replace this
	unsigned char txBuffer[1] = {0};
	long long size = 1;
	if(ft.transmit(txBuffer, size) != FTDI_OK)
		return SCAN_ER;
	// To here

	// Return if okay
	return SCAN_OK;
}
