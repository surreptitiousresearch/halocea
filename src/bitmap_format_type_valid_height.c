#include <stdint.h>
int bitmap_format_type_valid_height(int16_t format, int16_t type, int16_t height)
{
    if (format <= 0)
        return 0;
    if (format > 30000)
        return 0;
    return 1;
}
