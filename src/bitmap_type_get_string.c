/* bitmap_type_get_string @0x836F4240 */
#include <stdint.h>
#include "headers/bitmap_format_tables.h"

const char * bitmap_type_get_string(int16_t type)
{
    return bitmap_type_string_table[type];
}
