#include "ftdi.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SCAN_OK 0
#define SCAN_ER 1

class scanner
{
private:
	ftdi ft;

public:
	scanner();
	int connect();
	~scanner();
};
