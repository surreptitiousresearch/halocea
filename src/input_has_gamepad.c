/* input_has_gamepad @ 0x836FBC88 — a gamepad index is valid (present) when it is in [0,4).
 * Return attested uint8_t (Blam boolean): callers byte-normalize r3 (clrlwi 24 at 83688A9C etc.,
 * stbx at 83725684). */

#include <stdint.h>

uint8_t input_has_gamepad(int16_t gamepad_index)
{
    if ( gamepad_index < 0 )
        return 0;
    if ( gamepad_index >= 4 )
        return 0;
    return 1;
}
