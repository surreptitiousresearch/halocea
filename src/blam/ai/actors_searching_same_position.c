/* actors_searching_same_position @0x837F3970 — check whether two actors are both effectively searching/
 * pursuing the same spot. If both are in action class 5 or 7 (states whose action_data union carries a
 * pursuit_location at +8, e.g. search_state_data), and at least one has a non-zero pursuit_location.type:
 * type 1 (a specific firing position) matches only if both share the same firing_position_index; type 2
 * matches unconditionally. If NEITHER has a pursuit_location.type set, falls back to comparing their
 * associated props' body_position (actor+624, the "uncover/search target prop index" already established
 * in action_uncover_begin.c) — considered the same spot if within 0.7 units (squared distance < 0.49).
 * Returns false whenever either actor lacks a resolvable pursuit_location/prop or the pursuit types
 * disagree entirely. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/pursuit_location.h"
#include "headers/prop_datum.h"
#include "headers/pursuit_location_type.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);

uint8_t actors_searching_same_position(int actor_index, int friend_actor_index)
{
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
    actor_datum *friend_actor = DATUM_GET(actor_data, actor_datum, friend_actor_index);

    pursuit_location *actor_pursuit = 0;
    int16_t actor_action = actor->state.action;
    if ( actor_action == actor_action_search || actor_action == actor_action_uncover )
        actor_pursuit = &actor->state.action_data.___u0.search.pursuit_location;        /* actor+164 */

    pursuit_location *friend_pursuit = 0;
    int16_t friend_action = friend_actor->state.action;
    if ( friend_action == actor_action_search || friend_action == actor_action_uncover )
        friend_pursuit = &friend_actor->state.action_data.___u0.search.pursuit_location; /* friend+164 */

    if ( !actor_pursuit || !friend_pursuit )
        return 0;

    if ( actor_pursuit->type || friend_pursuit->type )
    {
        if ( actor_pursuit->type == _pursuit_location_position && friend_pursuit->type == _pursuit_location_position )
            return friend_pursuit->firing_position_index == actor_pursuit->firing_position_index;
        if ( actor_pursuit->type == _pursuit_location_undirected && friend_pursuit->type == _pursuit_location_undirected )
            return 1;
        return 0;
    }

    /* actor+624 = target.target_prop_index — associated search/uncover-target prop (see action_uncover_begin.c). */
    prop_datum *actor_prop = datum_try_and_get(prop_data, actor->target.target_prop_index);
    prop_datum *friend_prop = datum_try_and_get(prop_data, friend_actor->target.target_prop_index);
    if ( !actor_prop || !friend_prop )
        return 0;

    float dx = friend_prop->body_position.n[0] - actor_prop->body_position.n[0];
    float dy = friend_prop->body_position.n[1] - actor_prop->body_position.n[1];
    float dz = friend_prop->body_position.n[2] - actor_prop->body_position.n[2];

    return (dx * dx + dy * dy + dz * dz) < 0.49f;
}
