#include <iostream>
#include <fstream>
#include "scanner.h"
#include <vector>
#include <chrono>
#include <numeric>

using namespace std;

void scannerCallBack(vector<unsigned char>* data);

vector<unsigned char>* image_data = new vector<unsigned char>(2500);
uint8_t run = 0;
scanner scan;

int main()
{

	int result = -1;

	result = scan.init("GS1FPQH5");

	scan.scanStart(scannerCallBack);
}

// Scanner callback, lines will be received here
void scannerCallBack(vector<unsigned char>* data)
{
	image_data->clear();
	image_data->insert(image_data->begin(), data->begin(), data->end());

	//float average = accumulate(image_data->begin(), image_data->end(), 0.0)/image_data->size();

	//cout << average << endl;

	// A way to stop the scanning.
	if(run == 1) scan.scanStop();
}
