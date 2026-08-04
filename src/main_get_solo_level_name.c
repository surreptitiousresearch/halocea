#include <stdint.h>
#include "headers/blam_data_globals.h"
/* main_get_solo_level_name @0x836896E0 — return the scenario path for a solo level index (0..9), or null for
 * an out-of-range/negative index. */

/* param extsh r3 -> signed int16_t (matches DB hint); >=0 signed check then unsigned <0xA (clrlwi16) */
const char *main_get_solo_level_name(int16_t level)
{
    if ( level >= 0 && level < 0xA )
        return scenario_paths[level];
    return 0;
}
