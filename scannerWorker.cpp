#include "scannerWorker.h"


scannerWorker::scannerWorker()
{

}

scannerWorker::~scannerWorker()
{

}

void scannerWorker::lineWorker()
{
    // Set the loop to run
    this->loop_run = 1;

    // Create a place for the line
    uint8_t line[2047];
    uint32_t line_len, line_errors = 0;

    // Connect to the scanner
    scan.init("GS1FPQH5");

    // Start the hardware scanning
    scan.scanStart();

    // Loop
    while(loop_run == 1)
    {
        // Process the thread events from the controller
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

        // Get a new line
        int8_t result = scan.getLine(line, (uint32_t*)line_len);

        if(result != 0)
            std::cout << line_errors++ << std::endl;

        // Send the line to the GUI thread
        //emit lineDone(line);
    }

    // Quitting
    std::cout << "Thread stopped" << std::endl;
}

void scannerWorker::loopStop()
{
    this->loop_run = 0;
}
