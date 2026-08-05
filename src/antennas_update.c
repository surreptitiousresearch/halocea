/* antennas_update @0x8380DD38 — per-frame spring simulation step for every live antenna that isn't
 * flagged off, is attached to an object, and has rendered recently (fewer than 5 updates since last
 * render), with the timestep clamped to 1/15s. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/antenna_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void antenna_update(antenna_datum *antenna, antenna_definition *antenna_definition, float dt);

void antennas_update(float dt)
{
    for ( int antenna_index = data_next_index(antenna_data, -1); antenna_index != -1;
          antenna_index = data_next_index(antenna_data, antenna_index) )
    {
        antenna_datum *antenna = DATA_ARRAY_ELEMENT(antenna_data, antenna_datum, antenna_index);
        antenna_definition *definition = TAG_GET(antenna_definition, antenna->definition_index);

        if ( antenna->__noop )
            continue;

        int16_t updates_since_last_render = antenna->updates_since_last_render + 1;
        antenna->updates_since_last_render = updates_since_last_render;
        if ( antenna->object_index != -1 && updates_since_last_render < 5 )
        {
            float clamped_dt = dt;
            if ( dt > 0.06666667f )
                clamped_dt = 0.06666667f;
            antenna_update(antenna, definition, clamped_dt);
        }
    }
}
