/* input_get_key @0x836FBCB4 — pops the next buffered keystroke from the input ring, if any. Copies the
 * modifier flags + ascii code (one 16-bit word) and the key code into *key and advances the read index.
 * Returns 1 if a key was returned, 0 if the buffer is empty. */

#include <stdint.h>
#include "headers/input_globals.h"
#include "headers/key_stroke.h"

uint8_t input_get_key(key_stroke *key)
{
    if (input_globals.buffered_key_read_index < input_globals.buffered_key_write_index)
    {
        key_stroke *buffered = &input_globals.buffered_keys[input_globals.buffered_key_read_index];
        /* recovered: *(u16*)&key->modifier_flags = *(u16*)&buffered->modifier_flags -> per-member copy */
        key->modifier_flags = buffered->modifier_flags;
        key->ascii_code = buffered->ascii_code;
        key->key_code = buffered->key_code;
        ++input_globals.buffered_key_read_index;
        return 1;
    }
    return 0;
}
