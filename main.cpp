#include <iostream>
#include "ftdi.h"
#include "scanner.h"

using namespace std;

int main()
{
	ftdi ft;

	if(ft.connect("GSW1G8LTV") == FT_OK)
		cout << "Connected." << endl;

	else
		cout << "Could not connect." << endl;

	ft.disconnect();
}
