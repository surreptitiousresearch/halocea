/* unit_set_actively_controlled @ 0x836CEF38 — flag a unit as actively controlled (by a player or AI) and
 * propagate the state. A unit attached to an actor (unit.actor_index), swarm (unit.swarm_actor_index) or
 * controlling player (unit.player_index) is always considered controlled. Bits 0x1 and 0x40 of unit.flags
 * are set when controlled AND not destroyed (object.damage_flags bit 0x4), else cleared. Every weapon in
 * the unit's inventory is re-evaluated via item_in_unit_inventory, and the unit's seated children are
 * scanned to (re)assign the unit's driver/gunner occupant references based on each seat's flags (driver
 * bit 0x4, gunner bit 0x8) in the unit definition's seat block. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void item_in_unit_inventory(int item_index, int owner_unit_index);

void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( unit->unit.actor_index != -1 || unit->unit.swarm_actor_index != -1 || unit->unit.player_index != -1 )
        actively_controlled = 1;

    unsigned int flags = unit->unit.flags;
    /* not_destroyed: 0xFF when the destroyed bit is clear, 0x00 when set */
    uint8_t not_destroyed = ((unit->object.damage_flags & (1u << _object_dead_bit)) != 0) - 1;
    unsigned int with_bit0 = flags | (1u << _unit_actively_controlled_bit);
    if ( (not_destroyed & actively_controlled) == 0 )
        with_bit0 = flags & ~(1u << _unit_actively_controlled_bit);
    unit->unit.flags = with_bit0;
    unsigned int with_bit40 = with_bit0 | (1u << _unit_controllable_bit);
    if ( (not_destroyed & actively_controlled) == 0 )
        with_bit40 = with_bit0 & ~(1u << _unit_controllable_bit);
    unit->unit.flags = with_bit40;

    for ( int i = 0; i < 4; i = (int16_t)(i + 1) )
    {
        int weapon = unit->unit.weapon_object_indices[i];
        if ( weapon != -1 )
            item_in_unit_inventory(weapon, unit_index);
    }

    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, unit_data->definition_index);
    for ( int child = unit_data->object.first_child_object_index; child != -1; )
    {
        unit_datum *child_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
        int next_sibling = child_data->object.next_object_index;

        if ( ((1 << child_data->object.type) & object_mask_unit) != 0 )
        {
            int seat_index = child_data->unit.parent_seat_index;
            if ( seat_index != -1 )
            {
                unit_seat *seat = (unit_seat *)definition->unit.seats.address + seat_index;
                unsigned int seat_flags = seat->flags;

                if ( (seat_flags & (1u << _unit_seat_is_driver_bit)) != 0 && (unit_data->unit.flags & (1u << _unit_actively_controlled_bit)) == 0 && unit_data->unit.driver_object_index == -1 )
                {
                    unit_data->unit.driver_object_index = child;
                    if ( (seat->flags & (1u << _unit_seat_is_gunner_bit)) != 0 )
                    {
                        if ( unit_data->unit.gunner_object_index == -1 )
                            unit_data->unit.gunner_object_index = child;
                    }
                }
                else if ( (seat_flags & (1u << _unit_seat_is_gunner_bit)) != 0 )
                {
                    if ( unit_data->unit.gunner_object_index == -1
                      || unit_data->unit.gunner_object_index == unit_data->unit.driver_object_index )
                    {
                        unit_data->unit.gunner_object_index = child;
                    }
                }
            }
        }
        child = next_sibling;
    }
}
