/* unit_set_enterable_by_player @0x836CCDE8 — set whether a unit (vehicle) can be entered by a player (object
 * flags +0x204 bit 0x10000: set = NOT enterable). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


void unit_set_enterable_by_player(int unit_index, uint8_t enterable_by_player)
{
    if ( unit_index == -1 )
        return;
    unit_datum *object_data =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unsigned int flags = object_data->unit.flags;
    object_data->unit.flags = enterable_by_player
        ? (flags & ~(1u << _unit_not_enterable_by_player_bit))
        : (flags | (1u << _unit_not_enterable_by_player_bit));
}
