#include <iostream>
#include <fstream>
#include "scanner.h"
#include <vector>
#include <chrono>

using namespace std;

int main()
{
	scanner scan;
	ofstream myfile;

	int result = -1;
	result = scan.init("GS1FPQH5");

	result = scan.setScanEnable(1);

	uint32_t bytes = 5000;

	vector<unsigned char>* image_data = new vector<unsigned char>(bytes);


	int run = 0;



	unsigned long line_start = 0, line_finish = 0;

	uint32_t black_level = 39000;

	while(run == 0)
	{
		Sleep(10);

		result = scan.getData(image_data, bytes);

		for(unsigned long i = 0; i < bytes; i++)
		{
			if((int)(*image_data)[i] == 255)
			{
				line_finish = i - 1;

				unsigned long length = line_finish - line_start;

				if(length == 2046)
				{
					unsigned long line_average = 0;

					for(unsigned long i = 0; i < 2046; i++)
					{
						line_average += (int)(*image_data)[i + line_start];
					}

					cout << (line_average * 64) / 2046  << endl;
				}

				line_start = i + 1;
			}
		}

		scan.setBlackLevel(black_level);
	}





	myfile.open("image.txt");

	for(unsigned long i = 0; i < bytes; i++)
	{
		myfile << (int)(*image_data)[i] << endl;
	}

	myfile.close();



	scan.setScanEnable(0);
}
