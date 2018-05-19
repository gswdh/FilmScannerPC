#include <iostream>
#include "scanner.h"

using namespace std;

int main()
{


	scanner scan;

	int result = scan.connect("GS1FPQH5");

	uint32_t offset = 47513;
	double gain = 1;

	scan.setScanEnable(1);

	while(offset != 65536)
	{
		scan.setBlackLevel(offset);
		scan.setGain(gain);
	}



}
