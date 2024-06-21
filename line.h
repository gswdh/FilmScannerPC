#pragma once

#include <map>
#include <vector>

enum class line_pixel_format_t {
    LINE_PXL_FRMT_RGB48 = 0,
};

const std::map<line_pixel_format_t, std::uint32_t> bytes_per_pixel = {
    { line_pixel_format_t::LINE_PXL_FRMT_RGB48, 6 },
};

class Line {
private:
    std::vector<std::uint8_t> data;
    line_pixel_format_t format;

public:
    // Constructor
    Line(std::vector<std::uint8_t> data, const line_pixel_format_t format);

    // Meta
    uint32_t pixel_number(void) const;
};
