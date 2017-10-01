#include "scanner.h"

scanner::scanner()
{


}

void scanner::StartCall()
{

    // Start the scan
    this->StartScan(40000, 40000, 100, 500);
}

int scanner::StartScan(int gain, int offset, int brightness, long lines)
{

}


int scanner::StopScan()
{

}
