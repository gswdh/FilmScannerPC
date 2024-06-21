#pragma once

#include "scanner.h"

class Knokke : public Scanner {
private:
public:
    Knokke(std::string serial_number);

    // Override the area method
    scanner_status_t start(const scanner_scan_rate_t scan_rate) override;
    scanner_status_t stop(void) override;
};