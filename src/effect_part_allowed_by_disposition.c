#include <stdint.h>
/* effect_part_allowed_by_disposition @0x836E15B0 — decide whether an effect part may spawn given the
 * effect's violence disposition. A nonviolent effect suppresses parts with disposition "violent" (1);
 * a violent effect suppresses parts with disposition "nonviolent" (2). Any other disposition passes. */

int effect_part_allowed_by_disposition(uint8_t effect_is_nonviolent, int16_t part_disposition)
{
    int forbidden_disposition = effect_is_nonviolent ? 1 : 2;
    return part_disposition != forbidden_disposition;
}
