/* actor_perception_create_orphan_from_friend @0x837DBBD0 — promote an actor's perception of a unit into an
 * "orphan" prop (knowledge that outlives direct sensing), optionally seeded from a friend's acknowledged
 * prop. Resolves the base prop for the unit; unless it is mid-engagement (state 2 or 3, which forces a 0
 * return) it either creates a new orphan (via prop_orphan_transition, or prop_orphan_from_friend when a
 * friend prop is supplied — copying the unit/actor/swarm identity across) or refreshes the existing orphan
 * (resetting it to inspection state 4 with no friend, or pulling the friend's information otherwise). The
 * resulting prop's definite-knowledge source is set when the info came with no friend actor or from a
 * friend with sufficient visibility, and its unopposable flag and target weight are recomputed. Returns 1
 * on success, 0 if the prop was mid-engagement or an orphan could not be created. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/actor_position_data.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern int prop_orphan_transition(int actor_index, int parent_prop_index);
extern int prop_orphan_from_friend(int actor_index, int unacknowledged_prop_index, int friend_acknowledged_prop_index);
extern void prop_orphan_update_information(int actor_index, int orphan_prop_index, int friend_acknowledged_prop_index);
extern void prop_position_refresh(int actor_index, int prop_index, actor_position_data *sense_position, uint8_t forced_position_update, uint8_t full_status_update);
extern void prop_status_refresh(int actor_index, int prop_index, actor_position_data *sense_position);
extern uint8_t actor_compute_prop_unopposable(uint16_t actor_index, int prop_index);
extern float actor_compute_prop_target_weight(int actor_index, int prop_index);

static prop_datum *prop_from_index(int prop_index)
{
    return DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
}

int actor_perception_create_orphan_from_friend(int actor_index, int unit_index,
                                               int friend_actor_index, int friend_prop_index)
{
    int result = 1;
    int prop_index = prop_get_base_by_unit_index(actor_index, unit_index, 1, 0);
    if (prop_index == -1)
        return result;

    prop_datum *prop = prop_from_index(prop_index);

    if (prop->state < _prop_state_becoming_unacknowledged || prop->state > _prop_state_acknowledged)
    {
        int orphan_index = prop->___u3.orphan_prop_index;
        if (orphan_index == -1)
        {
            int new_index;
            if (friend_prop_index == -1)
            {
                actor_position_data sense_positions[2];
                uint8_t forced_position_update = 0;
                prop_position_refresh(actor_index, prop_index, sense_positions, forced_position_update, 0);
                new_index = prop_orphan_transition(actor_index, prop_index);
            }
            else
            {
                new_index = prop_orphan_from_friend(actor_index, prop_index, friend_prop_index);
                if (new_index != -1)
                {
                    prop_datum *new_prop = prop_from_index(new_index);
                    new_prop->unit_index = prop->unit_index;
                    new_prop->actor_index = prop->actor_index;
                    new_prop->swarm = prop->swarm;
                }
            }
            prop_index = new_index;
            if (new_index == -1)
                return 0;
            prop = prop_from_index(new_index);
        }
        else
        {
            uint8_t forced_position_update = 0;
            prop_datum *orphan = prop_from_index(orphan_index);
            if (friend_prop_index == -1)
            {
                orphan->orphan_inspection_ticks = 0;
                forced_position_update = 1;
                orphan->state = _prop_state_uninspected_orphan;
            }
            else
            {
                prop_orphan_update_information(actor_index, prop->___u3.orphan_prop_index, friend_prop_index);
                prop->unit_index = orphan->unit_index;
            }
            actor_position_data sense_position;
            prop_position_refresh(actor_index, orphan_index, &sense_position, forced_position_update, 1);
            prop_status_refresh(actor_index, orphan_index, &sense_position);
            prop_index = orphan_index;
            prop = orphan;
        }
    }
    else
    {
        result = 0;
    }

    if (prop)
    {
        unsigned char set_definite_knowledge = 0;
        if (friend_actor_index == -1
            || (friend_prop_index != -1 && prop_from_index(friend_prop_index)->visibility >= 2))
            set_definite_knowledge = 1;
        if (set_definite_knowledge)
        {
            prop->definite_knowledge_source_actor = friend_actor_index;
            prop->ticks_since_definitely_located = 0;
            prop->definitely_located = 1;
        }
        prop->unopposable_enemy = actor_compute_prop_unopposable(actor_index, prop_index);
        prop->target_weight = actor_compute_prop_target_weight(actor_index, prop_index);
    }

    return result;
}
