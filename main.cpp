#include <cassert>
#include <iostream>

#include "knokke.h"

int main(void)
{
    Knokke scanner(std::string("serial_number_123"));

    assert(scanner.start(scanner_scan_rate_t::SCNR_RATE_SINGLE) == scanner_status_t::SCNR_STATUS_SUCCESS);

    std::cout << "Passed" << std::endl;

    return 0;
}