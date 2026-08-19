/* motion_sensor_initialize_for_new_map @0x837BCAA8 — zero the motion-sensor globals, then mark every blip slot
 * (across both players, all 10 sensor data sets, 16 blips each) as _blip_type_none (empty/no blip).
 *
 * DEVIATION: the original walks the blip storage with one flat index — 346 blip-units per player (1384 bytes),
 * 33 per sensor datum (132 bytes) — which folds through motion_sensors[0].sensor_data[0]; same address, spelt. */

#include <string.h>
#include "headers/motion_sensor_globals.h"
#include "headers/blip_type.h"

void motion_sensor_initialize_for_new_map(void)
{
    motion_sensor_globals_definition *globals = motion_sensor_globals;
    memset(motion_sensor_globals, 0, sizeof(motion_sensor_globals_definition));

    for ( int player = 0; player < 2; ++player )
    {
        int datum_index = 0;
        int sensor = 10;
        do
        {
            int blip = 0;
            int count = 16;
            do
            {
                int blip_index = blip++;
                globals->motion_sensors[player].sensor_data[datum_index].blips[blip_index].type = _blip_type_none;
                --count;
            }
            while ( count );
            --sensor;
            ++datum_index;
        }
        while ( sensor );
    }
}
