/* cheat_active_camouflage_local_player @0x83699E68 — give a specific local player's unit full active
 * camouflage (same effect as cheat_active_camouflage but targeted by local-player slot 0/1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"

extern int local_player_get_player_index(int16_t local_player_index);

void cheat_active_camouflage_local_player(int16_t local_player_index)
{
    if ( (unsigned int)local_player_index > 1 )
        return;

    int player_index = local_player_get_player_index(local_player_index);
    if ( player_index == -1 )
        return;

    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int flags = unit_object->unit.flags;
    unit_object->unit.active_camouflage = 1.0f;
    if ( (flags & (1u << _unit_active_camouflaged_bit)) != 0 )
        unit_object->unit.flags = flags | (1u << _unit_super_camouflaged_bit);
    unit_object->unit.flags |= (1u << _unit_active_camouflaged_bit);
}
