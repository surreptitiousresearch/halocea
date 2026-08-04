/* motion_sensor_initialize_for_new_map @0x837BCAA8 — zero the motion-sensor globals, then mark every blip slot
 * (across both players, all 10 sensor data sets, 16 blips each) as _blip_type_none (empty/no blip).
 *
 * The original iterates the blip storage as a flat array: 346 blip-units per player (1384 bytes), 33 per sensor
 * datum (132 bytes), 16 live blips per datum. The nested member access below addresses that same flat memory. */

#include <string.h>
#include "headers/motion_sensor_globals.h"
#include "headers/blip_type.h"

void motion_sensor_initialize_for_new_map(void)
{
    motion_sensor_globals_definition *globals = motion_sensor_globals;
    memset(motion_sensor_globals, 0, sizeof(motion_sensor_globals_definition));

    for ( int player_base = 0; player_base < 692; player_base += 346 )
    {
        int datum_base = player_base;
        int sensor = 10;
        do
        {
            int blip = 0;
            int count = 16;
            do
            {
                int flat_index = datum_base + blip++;
                globals->motion_sensors[0].sensor_data[0].blips[flat_index].type = _blip_type_none;
                --count;
            }
            while ( count );
            --sensor;
            datum_base += 33;
        }
        while ( sensor );
    }
}
