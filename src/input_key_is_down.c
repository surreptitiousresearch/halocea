/* input_key_is_down @0x836FBB9C — return the down-duration (ticks held) for a key. Key codes 110-113 are
 * the generic modifier keys (shift/ctrl/alt/win) that map to either physical side; for these it returns
 * the larger tick count of the two physical keys. All other codes index key_ticks directly. */

#include <stdint.h>
#include "headers/input_globals.h"
#include "headers/key_code.h"

/* Attested uint8_t return: every return path byte-normalizes r3 (clrlwi ..,24 / lbzx) and
 * sampled callers re-normalize with clrlwi r3,24 / stbx (byte-normalization rule). */
uint8_t input_key_is_down(int16_t key_code)
{
    if ( (unsigned int)(key_code - _key_shift) <= 3 )
    {
        /* Generic (side-agnostic) modifier keys: return the larger held-tick count of the
         * two physical keys. Codes/indices from DB enum $433719FE2A052FABF0822A32CD2F9634. */
        switch ( key_code )
        {
            case _key_control:
            {
                unsigned char a = input_globals.key_ticks[_key_left_control];
                unsigned char b = input_globals.key_ticks[_key_right_control];
                return a > (unsigned int)b ? a : b;
            }
            case _key_windows:
            {
                unsigned char a = input_globals.key_ticks[_key_left_windows];
                unsigned char b = input_globals.key_ticks[_key_right_windows];
                return a > (unsigned int)b ? a : b;
            }
            case _key_shift:
            {
                unsigned char a = input_globals.key_ticks[_key_left_shift];
                unsigned char b = input_globals.key_ticks[_key_right_shift];
                return a > (unsigned int)b ? a : b;
            }
            default: /* _key_alt */
            {
                unsigned char a = input_globals.key_ticks[_key_left_alt];
                unsigned char b = input_globals.key_ticks[_key_right_alt];
                return a > (unsigned int)b ? a : b;
            }
        }
    }
    return input_globals.key_ticks[key_code];
}
