#include <stdint.h>
#include "headers/blam_data_globals.h"
/* actor_mode_name @0x837F3E00 — debug name of an actor mode index (0-3), or "unknown" out of range. */


const char * actor_mode_name(int16_t actor_mode_index)
{
    if ( actor_mode_index >= 0 && actor_mode_index < 4 )
        return global_actor_mode_names[actor_mode_index];
    return "unknown";
}
