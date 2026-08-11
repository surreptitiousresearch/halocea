/* actor_situation_update_target_status @0x837D71D8 — derive an actor's perceived target-status code
 * (actor +616) from the state of the prop (perception record) it is currently tracking (+624). With no
 * tracked prop the status is cleared. Otherwise the prop's engagement state (prop +36) plus assorted
 * prop flags select a status code (0..11) describing how the actor sees that target (e.g. just-seen,
 * firing, fleeing, dead, search states). It also sets a "target is exposed/visible" byte (actor +636)
 * and, for the active-combat states, latches the prop's tracked unit index (+620).
 *
 * Raw-offset access (actor 1828B, prop 312B). DEVIATION: the prop-state > 5 branch reads an
 * uninitialized stack slot in the decompiler; that engagement state never occurs (states are 0..5), so
 * the status is left unchanged here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/actor_target_type.h"
#include "headers/prop_perception_state.h"
#include "headers/ai_line_of_sight.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


void actor_situation_update_target_status(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (actor->target.target_prop_index == -1)
    {
        actor->target.target_type = actor_target_none;
        actor->target.target_last_visible_time = -1;
        actor->target.target_really_alive = 0;
        return;
    }

    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
    unsigned int engagement_state = (uint16_t)prop->state;
    object_datum *target_object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum;

    int16_t status = actor->target.target_type; /* preserved for the unreachable state>5 path */
    if (engagement_state > _prop_state_inspected_orphan)
    {
        /* unreachable; decompiler read an uninitialized slot here */
    }
    else if (engagement_state == _prop_state_becoming_acknowledged)
    {
        status = actor_target_partial_enemy;
    }
    else if (engagement_state == _prop_state_becoming_unacknowledged || engagement_state == _prop_state_acknowledged)
    {
        if (prop->dead)
            status = actor_target_dead_enemy;
        else if (prop->currently_damaging_me)
            status = actor_target_damaging_enemy;
        else if (prop->visibility < 2)
        {
            if ((uint16_t)prop->line_of_sight == _ai_line_of_sight_clear || (uint16_t)prop->line_of_sight == _ai_line_of_sight_occluded)
            {
                if (prop->quantized_facing > 2 || prop->distance >= 6.0f)
                    status = actor_target_clear_line_of_sight_enemy;
                else
                    status = actor_target_potentially_dangerous_enemy;
            }
            else
            {
                status = actor_target_acknowledged_enemy;
            }
        }
        else
        {
            status = actor_target_visible_enemy;
        }
    }
    else if (engagement_state == _prop_state_uninspected_orphan)
    {
        status = (prop->definitely_located != 0) + actor_target_uninspected_orphan;
    }
    else if (engagement_state) /* _prop_state_inspected_orphan */
    {
        if (prop->dead)
            status = actor_target_dead_enemy;
        else
            status = (prop->abandoned_search == 0) + actor_target_disregarded_orphan;
    }
    else /* _prop_state_unacknowledged — target lost */
    {
        status = actor_target_none;
        actor->target.target_prop_index = -1;
        actor->target.target_last_visible_time = -1;
    }

    actor->target.target_type = status;

    int state = prop->state;
    if (state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged)
    {
        actor->target.target_really_alive = ((target_object->object.damage_flags >> _object_dead_bit) & 1) == 0;
    }
    else
    {
        actor->target.target_really_alive = (prop->dead == 0);
        if (prop->visibility > 0)
            actor->target.target_last_visible_time = prop->last_visible_time;
    }
}
