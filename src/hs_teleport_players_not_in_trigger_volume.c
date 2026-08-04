/* hs_teleport_players_not_in_trigger_volume @0x837F88E8 — teleport every player whose unit is NOT inside the
 * given trigger volume to a scenario flag (reposition and reorient). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);
extern void hs_object_orient(int object_index, int16_t flag_index, uint8_t position, uint8_t facing);

void hs_teleport_players_not_in_trigger_volume(int16_t trigger_volume_index, int16_t flag_index)
{
    for ( int i = data_next_index(player_data, -1); i != -1; i = data_next_index(player_data, i) )
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, i);
        int unit_index = player->unit_index;
        if ( unit_index != -1 && !scenario_trigger_volume_test_object(trigger_volume_index, unit_index) )
            hs_object_orient(unit_index, flag_index, 1u, 1u);
    }
}
