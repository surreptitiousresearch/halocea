/* get_nav_point_datum @0x837ECEE8 */
#include <stdint.h>
#include "headers/hud_nav_point_player_datum.h"
#include "headers/blam_data_globals.h"


hud_nav_point_player_datum * get_nav_point_datum(int16_t local_player_index)
{
    return &nav_point_data[local_player_index];
}
