/* ai_handle_deleted_object @0x836E9258 — AI-side cleanup hook run when `deleted_object_index` is about to be
 * deleted. No-op unless AI is initialized for the current map, and unless the deleted object is a biped or a
 * vehicle (object.type 0 or 1). For those: releases its controlling actor (actor_delete, or notifies its
 * swarm actor if it only had a swarm parent), scrubs every prop's references to it (both as the prop's own
 * unit and, separately, as a vehicle a prop was gunning/driving), notifies conversations of the death, and
 * compacts the AI global mounted-weapon-unit list by removing any entry pointing at it (swap-with-last).
 *
 * The controlling-actor/swarm-actor pair is the typed unit.actor_index / unit.swarm_actor_index
 * (unit block +0/+4, i.e. object +500/+504; see _unit_datum.h). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/ai_globals.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/data_iterator.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern void actor_delete(int actor_index, uint8_t died);
extern void actor_swarm_unit_died(uint16_t actor_index, int unit_index);
extern void actor_switch_props(uint16_t actor_index, int prop_index, int replacement_prop_index);
extern void prop_delete(int actor_index, int prop_index);
extern void ai_conversation_unit_died(int unit_index, uint8_t deleted);

void ai_handle_deleted_object(int deleted_object_index)
{
    if (!ai_globals->ai_initialized_for_map)
        return;

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, deleted_object_index)->datum;
    char *object_raw = (char *)object;

    if (((1 << object->object.type) & object_mask_unit) == 0)
        return;

    int controlling_actor_index = ((unit_datum *)object_raw)->unit.actor_index;
    if (controlling_actor_index == -1)
    {
        int swarm_actor_index = ((unit_datum *)object_raw)->unit.swarm_actor_index;
        if (swarm_actor_index != -1)
            actor_swarm_unit_died(swarm_actor_index, deleted_object_index);
    }
    else
    {
        actor_delete(controlling_actor_index, 0);
    }

    data_iterator prop_iterator;
    data_iterator_new(&prop_iterator, prop_data);
    for (prop_datum *prop = data_iterator_next(&prop_iterator); prop;
            prop = data_iterator_next(&prop_iterator))
    {
        if (prop->unit_index == deleted_object_index)
        {
            actor_switch_props(prop->owner_actor_index, prop_iterator.index, -1);
            prop_delete(prop->owner_actor_index, prop_iterator.index);
        }
        else if (prop->vehicle_index == deleted_object_index)
        {
            prop->vehicle_index = -1;
            prop->dangerous_vehicle_driver = 0;
            prop->vehicle_gunner = 0;
        }
    }

    ai_conversation_unit_died(deleted_object_index, 1);

    for (__int16 i = 0; i < ai_globals->mounted_weapon_unit_count; ++i)
    {
        if (ai_globals->mounted_weapon_unit_indices[i] == deleted_object_index)
        {
            --ai_globals->mounted_weapon_unit_count;
            if (ai_globals->mounted_weapon_unit_count > 0)
                ai_globals->mounted_weapon_unit_indices[i] =
                        ai_globals->mounted_weapon_unit_indices[ai_globals->mounted_weapon_unit_count];
        }
    }
}
