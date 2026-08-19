#include <stdint.h>
#include "headers/blam_data_globals.h"
/* main_get_difficulty @0x83689000 — return the current global game difficulty level. */

/* ret int16_t: lhz load (16-bit) + caller extsh (sign-extend) at 83685290 => signed 16-bit. */
int16_t main_get_difficulty(void)
{
    return global_difficulty_level;
}
