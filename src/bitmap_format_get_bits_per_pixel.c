/* bitmap_format_get_bits_per_pixel @0x836F4270 */
#include <stdint.h>
#include "headers/bitmap_format_tables.h"

/* return int16_t: all 3 callers extsh r3 after the call; callee loads a byte table (lbzx+extsb
 * @836F427C/836F4280 — see the filed bitmap_format_bits_per_pixel_table int8_t header finding). */
int16_t bitmap_format_get_bits_per_pixel(int16_t format)
{
    return bitmap_format_bits_per_pixel_table[format];
}
