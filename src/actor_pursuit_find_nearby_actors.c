/* actor_pursuit_find_nearby_actors @0x837F3BD0 — count the friendly actors near this actor that are eligible to
 * join a pursuit, and record the closest one's prop index in the actor's pursuit order. First it walks the
 * actor's own prop list: a prop that is a non-dead friend with a valid actor, and (when a coordinator is set)
 * in a moving state [2,3], is offered to actor_pursuit_consider_nearby_actor; accepted props are counted and
 * the nearest (by prop distance) is remembered. If fewer than the required number were found (2 when
 * coordinating, else 1), it widens the search across the whole encounter, converting each accepted encounter
 * member to a prop (active, else a freshly based one) and remembering the nearest by true 3D body distance,
 * stopping once the quota is met. The best prop index is stored to external_orders.pursuit_group_prop_index;
 * returns the number found.
 *
 * DEVIATION: actor typed as actor_datum* to name meta.encounter_index (+52), input.position.body_position
 * (+300) and external_orders.pursuit_group_prop_index (+464). Both actor_pursuit_consider_nearby_actor call
 * sites were traced in disasm — r3=pursuit_coordinator (the callee's u8 slot id), r4=the candidate actor
 * index; the callee ignores its third argument (a stale r5), so 0 is passed. The cntlzw count idiom and
 * register-pun distance math are written in plain form. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern float __fsqrts(float);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t actor_pursuit_consider_nearby_actor(uint8_t actor_index, int pursuit_coordinator, int friend_actor_index);
extern int prop_get_active_by_unit_index(int actor_index, int unit_index);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);

int actor_pursuit_find_nearby_actors(int actor_index, int pursuit_coordinator)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uint8_t is_coordinator = (uint8_t)pursuit_coordinator;
    int required_count = (is_coordinator != 0) + 1;   /* 2 when coordinating, else 1 */
    int found_count = 0;
    float nearest_distance = 3.4028235e38f;
    int best_prop_index = -1;

    encounter_actor_iterator iterator;
    prop_iterator *props = (prop_iterator *)&iterator;

    prop_iterator_new(props, actor_index);
    for ( prop_datum *prop = prop_iterator_next(props); prop; prop = prop_iterator_next(props) )
    {
        if ( prop->enemy || prop->dead )
            continue;
        if ( prop->actor_index == -1 )
            continue;
        if ( is_coordinator )
        {
            int16_t state = prop->state;
            if ( state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged )
                continue;
        }
        if ( actor_pursuit_consider_nearby_actor(pursuit_coordinator, prop->actor_index, 0) )
        {
            ++found_count;
            if ( prop->distance < nearest_distance )
            {
                best_prop_index = props->index;
                nearest_distance = prop->distance;
            }
        }
    }

    if ( found_count < required_count )
    {
        int encounter_index = actor->meta.encounter_index;
        if ( encounter_index != -1 )
        {
            encounter_actor_iterator_new(&iterator, encounter_index);
            for ( actor_datum *friend_actor = encounter_actor_iterator_next(&iterator); friend_actor;
                    friend_actor = encounter_actor_iterator_next(&iterator) )
            {
                if ( friend_actor->meta.unit_index == -1 )
                    continue;
                if ( !actor_pursuit_consider_nearby_actor(pursuit_coordinator, iterator.index, 0) )
                    continue;

                int friend_prop_index = prop_get_active_by_unit_index(actor_index, friend_actor->meta.unit_index);
                if ( friend_prop_index == -1 )
                {
                    friend_prop_index = prop_get_base_by_unit_index(actor_index, friend_actor->meta.unit_index, 1, 0);
                    if ( friend_prop_index == -1 )
                        continue;
                }

                ++found_count;
                float dz = friend_actor->input.position.body_position.n[2] - actor->input.position.body_position.n[2];
                float dx = friend_actor->input.position.body_position.n[0] - actor->input.position.body_position.n[0];
                float dy = friend_actor->input.position.body_position.n[1] - actor->input.position.body_position.n[1];
                float distance = __fsqrts(dy * dy + (dx * dx + dz * dz));
                if ( distance < nearest_distance )
                {
                    best_prop_index = friend_prop_index;
                    nearest_distance = distance;
                }
                if ( found_count >= required_count )
                    break;
            }
        }
    }

    actor->external_orders.pursuit_group_prop_index = best_prop_index;
    return found_count;
}
