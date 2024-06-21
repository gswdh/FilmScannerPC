#include "line.h"

Line::Line(std::vector<std::uint8_t> data, const line_pixel_format_t format)
{
    this->data = data;
    this->format = format;
}

uint32_t Line::pixel_number(void) const
{
    return data.size() / bytes_per_pixel.at(format);
}
