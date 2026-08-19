/* get_motion_sensor_data @0x837BCA48 */
/* DEVIATION: return type corrected to motion_sensor_player_datum* — the binary returns
 * motion_sensor_globals + 0x568*local_player_index (0x837BCA50 mulli r10,r11,0x568;
 * 0x837BCA58 add r3,r10,r11) = &motion_sensors[local_player_index], one 1384-byte player
 * datum; a motion_sensor_globals_definition* is wrong for index 1 (mid-struct). */
#include <stdint.h>
#include "headers/motion_sensor_globals_definition.h"

motion_sensor_player_datum * get_motion_sensor_data(int16_t local_player_index)
{
    return &motion_sensor_globals->motion_sensors[local_player_index];
}
