#pragma once

#include <string>
#include <vector>

#include "line.h"

enum class scanner_status_t {
    SCNR_STATUS_SUCCESS = 0,
    SCNR_STATUS_FAIL,
    SCNR_STATUS_NOT_IMPLEMENTED,
};

enum class scanner_scan_rate_t {
    SCNR_RATE_SINGLE = 0,
    SCNR_RATE_DOUBLE,
};

class Scanner {

private:
    std::vector<Line> lines;
    scanner_scan_rate_t scan_rate;

public:
    // Constructor
    Scanner(std::string serial_number);

    // High level
    scanner_status_t start(const scanner_scan_rate_t scan_rate);
    scanner_status_t stop(void);

    // Backlight
    scanner_status_t get_backlight_brightness(const float* brightness) const;
    scanner_status_t set_backlight_brightness(const float brightness);

    // Motor in units of mm/s film speed
    scanner_status_t get_motor_speed(const float* speed) const;
    scanner_status_t set_motor_speed(const float speed);

    // Sensor public
    scanner_status_t get_scan_rate(const scanner_scan_rate_t* scan_rate) const;
    scanner_status_t set_scan_rate(const scanner_scan_rate_t scan_rate);

    // Data interface
    scanner_status_t get_line(const Line* line);
};
