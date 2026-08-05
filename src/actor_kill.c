/* actor_kill @0x837203D0 — kill the unit(s) an actor controls. For a swarm actor every member unit in the
 * object child-chain is flagged dead (object flag 0x40, or 0x20 when not silent) and detached; for a normal
 * actor its single unit is flagged and detached. Unless delayed, the actor datum is then deleted and its
 * encounter status refreshed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern void actor_swarm_detach_from_unit(uint16_t actor_index, int unit_index);
extern void actor_detach_from_unit(uint16_t actor_index);
extern void actor_delete(int actor_index, uint8_t died);
extern void encounter_update_status(int encounter_index);

void actor_kill(int actor_index, uint8_t silent, uint8_t delayed)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int encounter_index = actor->meta.encounter_index;

    if ( actor->meta.swarm )
    {
        int unit = actor->meta.swarm_unit_index;
        if ( unit != -1 )
        {
            do
            {
                unit_datum *object = (unit_datum *)
                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit)->datum;
                int16_t flags = object->object.damage_flags;
                object->object.damage_flags = flags | (silent ? (1 << _object_die_act_of_god_silent_bit)
                                                       : (1 << _object_die_act_of_god_bit));
                if ( !delayed )
                    actor_swarm_detach_from_unit(actor_index, unit);
                unit = object->unit.swarm_next_unit_index;
            }
            while ( unit != -1 );
        }
    }
    else
    {
        object_datum *object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum;
        int16_t flags = object->object.damage_flags;
        object->object.damage_flags = flags | (silent ? (1 << _object_die_act_of_god_silent_bit)
                                                       : (1 << _object_die_act_of_god_bit));
        if ( !delayed )
            actor_detach_from_unit(actor_index);
    }

    if ( !delayed )
    {
        actor_delete(actor_index, 1u);
        if ( encounter_index != -1 )
            encounter_update_status(encounter_index);
    }
}
