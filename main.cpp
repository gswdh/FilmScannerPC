#include <iostream>
#include <fstream>
#include "scanner.h"
#include <vector>
#include <chrono>
#include <numeric>
#include <stdio.h>

using namespace std;

void scannerCallBack(vector<unsigned char>* data);

//vector<unsigned char>* line = new vector<unsigned char>(2500);
uint8_t* image_data = (uint8_t*)malloc (3000*2047);
uint8_t run = 0;
scanner scan;
FILE* pFile;

uint16_t lines_done = 0;

int main()
{
	int result = -1;

	char buffer [50];
	uint8_t bytes = sprintf(buffer, "P5 2047 3000 255 \n");

	pFile = fopen("image.pgm", "wb");
	fwrite(buffer, 1, bytes*sizeof(uint8_t), pFile);
	fclose(pFile);

	result = scan.init("GS1FPQH5");

	scan.scanStart(scannerCallBack);
}

// Scanner callback, lines will be received here
void scannerCallBack(vector<unsigned char>* data)
{
	//image_data->clear();
	//image_data->insert(image_data->end(), data->begin(), data->end());

	memcpy(&image_data[lines_done * 2047], &data[0], 2047 * sizeof(uint8_t));

	if(lines_done == 3000)
	{
		run = 1;

		cout << "Done" << endl;
	}

	else
	{
		//pFile = fopen("image.pgm", "a");
		//fwrite(image_data, 1, image_data->size()*sizeof(uint8_t), pFile);
		//fclose(pFile);

		lines_done++;
	}

	// A way to stop the scanning.
	if(run == 1)
	{
		scan.scanStop();
		pFile = fopen("image.pgm", "a");
		fwrite(image_data, 1, 3000*2047*sizeof(uint8_t), pFile);
		fclose(pFile);
	}
}
