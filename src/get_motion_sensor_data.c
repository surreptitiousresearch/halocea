#include <stdint.h>
#include "headers/motion_sensor_globals_definition.h"

motion_sensor_globals_definition * get_motion_sensor_data(int16_t local_player_index)
{
    /* recovered: (char *)motion_sensor_globals + 1384 * index -> &motion_sensors[index] */
    return (motion_sensor_globals_definition *)
        &motion_sensor_globals->motion_sensors[local_player_index];
}
