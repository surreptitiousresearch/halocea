/* actor_berserk @0x837D8868 — set or clear an actor's "berserk" flag (actor->emotions.berserk) and
 * propagate the matching berserk bit (0x80) onto its controlled unit object(s). For a swarm actor
 * (actor->meta.swarm) it walks the swarm's unit object chain (head at actor->meta.swarm_unit_index,
 * next at unit_datum.unit.swarm_next_unit_index) and ORs bit 0x80 into object_datum.object.damage_flags.
 * For a single-unit actor it sets or clears bit 0x80 in unit_datum.unit.flags to match the new berserk
 * value. Setting berserk also raises actor->emotions.forced_to_charge.
 *
 * Object data block pointers come from the object datum array (12-byte elements; data pointer at +8), reproduced
 * with the same datum-index arithmetic used elsewhere. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_berserk(int actor_index, uint8_t berserk)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( berserk == actor->emotions.berserk )
        return;

    unsigned char is_swarm = actor->meta.swarm;
    actor->emotions.berserk = berserk;
    actor->emotions.played_berserk_sound = 0;

    if ( is_swarm )
    {
        for ( int unit_object_index = actor->meta.swarm_unit_index; unit_object_index != -1; )
        {
            unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_object_index)->datum);
            unit_object_index = object->unit.swarm_next_unit_index;
            object->object.damage_flags |= (1u << _object_melee_attack_inhibited_bit);
        }
    }
    else
    {
        int unit_object_index = actor->meta.unit_index;
        unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_object_index)->datum);
        unsigned int flags = object->unit.flags;
        object->unit.flags = berserk ? (flags | (1u << _unit_ignore_hard_pings_bit))
                                     : (flags & ~(1u << _unit_ignore_hard_pings_bit));
    }

    if ( berserk )
        actor->emotions.forced_to_charge = 1;
}
