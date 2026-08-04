#include <stdint.h>
#include "headers/bitmap_format_tables.h"

const char * bitmap_format_get_string(int16_t format)
{
    return bitmap_format_string_table[format];
}
