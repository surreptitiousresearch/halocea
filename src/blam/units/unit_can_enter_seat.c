/* unit_can_enter_seat @0x836C86E8 — whether a unit may board a particular seat of a parent unit (vehicle).
 * Walks the parent's seated occupants (child chain: object.first_child_object_index / object.next_object_index;
 * seat index at unit.parent_seat_index). Entry is denied (returns 0) if the unit would board its own seat, if the
 * target seat is already taken (also reported through occupant_unit_index_reference), or if the entering unit is
 * player-controlled (unit.player_index) and any current occupant is on an enemy team (object.owner_team_index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);

uint8_t unit_can_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index, int *occupant_unit_index_reference)
{
    int occupant_unit_index = -1;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int can_enter = parent_unit_index != unit_index;

    unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum);
    for ( int child_index = parent->object.first_child_object_index; child_index != -1; )
    {
        unit_datum *child = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_index)->datum;
        if ( ((1 << (unsigned short)child->object.type) & 3) != 0 )   /* child is a unit */
        {
            if ( child->unit.parent_seat_index == seat_index )
            {
                can_enter = 0;
                occupant_unit_index = child_index;
            }
            else if ( unit->unit.player_index != -1 )
            {
                unsigned char is_enemy = game_team_is_enemy(unit->object.owner_team_index, child->object.owner_team_index);
                can_enter = is_enemy != 0 ? 0 : can_enter;
            }
        }
        child_index = child->object.next_object_index;
    }

    if ( occupant_unit_index_reference )
        *occupant_unit_index_reference = occupant_unit_index;
    return can_enter;
}
