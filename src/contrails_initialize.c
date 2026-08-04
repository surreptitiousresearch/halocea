/* contrails_initialize @ 0x8373FB18 — contrail + contrail-point pools.
 * If either pool fails to allocate, both are released (kept consistent). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void contrails_initialize(void)
{
    contrail_data = game_state_data_new("contrail", 256, 68);
    contrail_point_data = game_state_data_new("contrail point", 1024, 56);
    if ( contrail_data )
    {
        if ( !contrail_point_data )
            contrail_data = 0;
    }
    else if ( contrail_point_data )
    {
        contrail_point_data = 0;
    }
}
