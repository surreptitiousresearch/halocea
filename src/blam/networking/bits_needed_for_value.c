/* bits_needed_for_value @0x837A0F68 */
/* DEVIATION: the local extern typed bits_needed as `int[]`, a 4x stride error — the table is a byte
 * array, read here with `lbzx r3, r10, r3` (unscaled index, disasm site 0x837A0F68). Declared
 * canonically by headers/blam_data_globals.h (const uint8_t[2048], def src/data/bits_needed.c). */
#include "headers/blam_data_globals.h"

int bits_needed_for_value(int value)
{
    return bits_needed[value];
}
