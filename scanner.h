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

public:
    scanner_scan_rate_t scan_rate;

    // Constructor
    Scanner(std::string serial_number);

    // High level
    virtual scanner_status_t start(const scanner_scan_rate_t scan_rate);
    virtual scanner_status_t stop(void);

    // Backlight
    virtual scanner_status_t get_backlight_brightness(const float* brightness) const;
    virtual scanner_status_t set_backlight_brightness(const float brightness);

    // Motor in units of mm/s film speed
    virtual scanner_status_t get_motor_speed(const float* speed) const;
    virtual scanner_status_t set_motor_speed(const float speed);

    // Sensor public
    virtual scanner_status_t get_scan_rate(const scanner_scan_rate_t* scan_rate) const;
    virtual scanner_status_t set_scan_rate(const scanner_scan_rate_t scan_rate);

    // Data interface
    virtual scanner_status_t get_line(const Line* line);
};
