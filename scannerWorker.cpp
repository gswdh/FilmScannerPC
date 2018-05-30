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

    // Loop
    while(loop_run == 1)
    {
        // Process the thread events from the controller
        QTime dieTime = QTime::currentTime().addMSecs(1);
        while(QTime::currentTime() < dieTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        }

        // Do the intensive blocking work

    }

    // Quitting
    std::cout << "Thread stopped" << std::endl;
}

void scannerWorker::loopStop()
{
    this->loop_run = 0;
}
