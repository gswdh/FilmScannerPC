#include "mainwindow.h"
#include "ui_mainwindow.h"

ftdi ft;
scanner sc;
FTDIData cont_data;
int running = 0;
pthread_t scanner_thread;
int t1;
unsigned long nlines_global;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->frames_lineedit->setText("0");

    QImage img;

    img.fill(QColor(Qt::black).rgb());

    ui->preview_label->setPixmap(QPixmap::fromImage(img));

    ui->preview_label->show();


}

MainWindow::~MainWindow()
{
    // Disconnect the scanner properly if it's still running
    if(running == 1)
    {
        cont_data.buff_len = 32;
        cont_data.buffer[0] = 0;
        ft.Write(cont_data);
        ft.Disconnect();
    }

    delete ui;
}

void *collectdata(void *argument)
{
    // Set up the scan
    unsigned long nlines = nlines_global;
    unsigned char line[2049];
    unsigned int buff_pntr = 0, line_pntr = 0;
    FTDIData read_data;

    // Change the state machine
    running = 1;

    // If we still want to collect data
    while(nlines != 0)
    {
        // Get some data
        ft.Read(&read_data);

        // Go through the data
        while(buff_pntr < read_data.buff_len)
        {
            // Come to a line end
            if(read_data.buffer[line_pntr] == 255)
            {
                // Check to see if we have a complete line (drop it if not)
                if(line_pntr == 2048)
                {
                    // Do something with the line

                    // Decrement nlines
                    nlines--;
                }

                // Reset the line pointer
                line_pntr = 0;
            }

            // Carry on extracting data
            line[line_pntr++] = read_data.buffer[buff_pntr++];
        }

        // Reset the pointer
        buff_pntr = 0;
    }

    // Finish
    cont_data.buffer[0] = 0;
    ft.Write(cont_data);

    // Disconnect from the scanner
    ft.Disconnect();

    // Change the UI state
    //ui->startstop_button->setText("Start");

    //pthread_join(scanner_thread, NULL);

    // Announce on the console
    std::cout << "Disconnected" << std::endl;

}

void MainWindow::on_refresh_button_clicked()
{
    // Create a new device list
    FTDIDevices list;

    ft.ListDevices(&list);

    std::cout << "Number of devices attached: " << list.ndevs << std::endl;
    std::cout << list.device1 << std::endl;
    std::cout << list.device2 << std::endl;

    ui->devices_box->clear();

    ui->devices_box->addItem(QString(list.device1));
    ui->devices_box->addItem(QString(list.device2));
}

void MainWindow::on_startstop_button_clicked()
{
    //emit startstop_button_clicked(ui->devices_box->itemData( ui->devices_box->currentIndex()));

    // Create the start data
    cont_data.buff_len = 32;
    cont_data.buffer[0] = 0;

    // Get the scan settings from the UI


    // Start the scan
    if(running == 0)
        this->StartScanning();

    // Stop the scan
    else
        this->StopScanning();

}

void MainWindow::StartScanning(void)
{
    // Attempt to connect to the scanner
    if(ft.Connect("GS1FPQH5") == 0)
    {
        // Annouce
        std::cout << "Connected" << std::endl;

        // Set the number of lines to scan
        nlines_global = ((ui->frames_lineedit->text().toInt()) * 1.1 * 3000) + 500;
        std::cout << "Lines to scan: " << nlines_global << std::endl;

        // Initiate a scan with the correct settings
        cont_data.buffer[0] = 1;
        ft.Write(cont_data);

        // Set the UI
        ui->startstop_button->setText("Stop");

        // Spawn the thread to collect the data
        t1 = pthread_create(&scanner_thread, NULL, collectdata, (void*)"");
    }

    // Otherwise we can't scan
    else std::cout << "Could not connect to scanner" << std::endl;
}

void MainWindow::StopScanning(void)
{
    // Stop the scanner
    cont_data.buffer[0] = 0;
    ft.Write(cont_data);

    // Disconnect from the scanner
    ft.Disconnect();

    // Change the UI state
    ui->startstop_button->setText("Start");

    //pthread_join(scanner_thread, NULL);

    // Announce on the console
    std::cout << "Disconnected" << std::endl;
}


