/* game_engine_player_depower_active_camo @0x83748E90 */
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void game_engine_player_depower_active_camo(int player_index)
{
    if ( player_index == -1 )
        return;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;
    if ( unit_index == -1 )
        return;

    unit_datum *object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( object->unit.flags & (1u << _unit_active_camouflaged_bit) )
        object->unit.active_camouflage = 0.5f;
}
