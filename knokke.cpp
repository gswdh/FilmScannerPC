#include "knokke.h"

Knokke::Knokke(std::string serial_number)
    : Scanner(serial_number)
{
}

// Override the area method
scanner_status_t Knokke::start(const scanner_scan_rate_t scan_rate)
{
    this->scan_rate = scan_rate;
    return scanner_status_t::SCNR_STATUS_SUCCESS;
}
scanner_status_t Knokke::stop(void)
{
    return scanner_status_t::SCNR_STATUS_SUCCESS;
}