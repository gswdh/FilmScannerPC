#include <iostream>
#include <fstream>
#include "scanner.h"
#include <vector>

using namespace std;

int main()
{
	scanner scan;
	ofstream myfile;

	int result = -1;
	result = scan.init("GS1FPQH5");

	result = scan.setScanEnable(1);

	uint32_t bytes = 2048;

	vector<unsigned char>* image_data = new vector<unsigned char>(bytes);

	result = scan.getData(image_data, bytes);

	myfile.open("image.txt");

	for(unsigned long i = 0; i < bytes; i++)
	{
		myfile << (int)(*image_data)[i] << endl;
	}

	myfile.close();

	scan.setScanEnable(0);
}
