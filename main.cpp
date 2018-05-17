#include <iostream>
#include "scanner.h"

using namespace std;

int main()
{


	scanner scan;

	int result = scan.connect("GS1FPQH5");

	scan.scan(0);
	scan.scan(1);
	scan.scan(0);
	scan.scan(1);
	scan.scan(0);
	scan.scan(1);




}
