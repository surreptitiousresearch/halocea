#include <stdint.h>
/* bitmap_format_type_valid_depth @ 0x836F4C58 — validate a bitmap type/depth pair:
 * type must be in (0, 256], and either the type is 1 (2D) or the format is 1.
 * (The depth argument is not consulted in this build.) */

int bitmap_format_type_valid_depth(int16_t format, int16_t type, int16_t depth)
{
    return type > 0 && type <= 256 && (type == 1 || format == 1);
}
