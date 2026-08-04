/* glow_initialize @0x8380C288 — one-time allocation of the glow and glow-particle datum arrays. */

#include <stdint.h>
#include "headers/glow_globals.h"

#include "headers/data_array.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void glow_initialize(void)
{
    if ( !glow_globals.glow_data )
    {
        glow_globals.glow_data = game_state_data_new("glow", 8, 604);
        if ( glow_globals.glow_data )
        {
            if ( !glow_globals.glow_particle_data )
                glow_globals.glow_particle_data = game_state_data_new("glow particles", 512, 100);
        }
    }
}
