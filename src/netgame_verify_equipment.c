/* netgame_verify_equipment @0x8374E830 — debug-build validation stub; the release build strips the
 * assertion body, leaving only the netgame_equipment scan that the compiler could not prove side-effect-free. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


void netgame_verify_equipment(int16_t game_type, char *string)
{
    int count = global_scenario->netgame_equipment.count;
    for ( int16_t i = 0; i < count; ++i )
        ;
}
