/* slayer_engine_initialize_for_new_map @0x83815358 — zero the live and baseline slayer score tables for a
 * new map. */

#include <string.h>
#include "headers/slayer_globals.h"
#include "headers/blam_data_globals.h"


unsigned __int8 slayer_engine_initialize_for_new_map(void)
{
    memset(&slayer_globals, 0, 0x40u);
    memset(slayer_globals.individual_score, 0, sizeof(slayer_globals.individual_score));
    memset(&slayer_globals_baseline, 0, 0x40u);
    memset(slayer_globals_baseline.individual_score, 0, sizeof(slayer_globals_baseline.individual_score));
    return 1;
}
