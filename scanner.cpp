#include "scanner.h"

#include <cassert>

Scanner::Scanner(std::string serial_number)
{
    (void)serial_number;
}

scanner_status_t Scanner::start(const scanner_scan_rate_t scan_rate)
{
    this->scan_rate = scan_rate;
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t Scanner::stop(void)
{
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t
Scanner::get_backlight_brightness(const float* brightness) const
{
    assert(brightness != NULL);
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t Scanner::set_backlight_brightness(const float brightness)
{
    (void)brightness;
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t Scanner::get_motor_speed(const float* speed) const
{
    assert(speed != NULL);
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t Scanner::set_motor_speed(const float speed)
{
    (void)speed;
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t
Scanner::get_scan_rate(const scanner_scan_rate_t* scan_rate) const
{
    assert(scan_rate != NULL);
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t Scanner::set_scan_rate(const scanner_scan_rate_t scan_rate)
{
    (void)scan_rate;
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}

scanner_status_t Scanner::get_line(const Line* line)
{
    assert(line != NULL);
    return scanner_status_t::SCNR_STATUS_NOT_IMPLEMENTED;
}
