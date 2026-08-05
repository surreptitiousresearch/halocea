/* ai_enemies_endanger_player @0x836E9610 — scan all AI "props" (the perception records the AI keeps on
 * the player) and decide whether any living enemy currently endangers the player. Returns 1 as soon as a
 * qualifying threat is found, 0 if none do.
 *
 * `must_be_attacking` (set by the ai_enemies_attacking_player wrapper) tightens the test: an enemy that is
 * merely aware counts when it is 0, but when it is 1 the enemy must actually be in an attacking posture.
 *
 * A prop qualifies as endangering the player when, for its owning actor and object:
 *   - the prop is the player (prop->player) and an enemy (prop->enemy), and its unit is player-controlled;
 *   - it is not already excluded by the "engageable / clearly attacking" early-accept (which would instead
 *     set the local `engaging` flag and skip the danger checks); and
 *   - one of: it recently had eyes on the player (last_visible_time within 90 ticks); it is very close
 *     (distance < 4); or it is specifically targeting this prop (actor target_prop_index == prop index) in a
 *     threatening state (search/combat states 2..5, with extra range/visibility gating for states 4..5). */

#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/actor_action.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


extern int game_time_get(void);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

uint8_t ai_enemies_endanger_player(uint8_t must_be_attacking)
{
    int current_time = game_time_get();

    data_iterator iterator;
    data_iterator_new(&iterator, prop_data);

    prop_datum *prop = data_iterator_next(&iterator);
    if ( !prop )
        return 0;

    while ( 1 )
    {
        if ( prop->player && prop->enemy )
        {
            int prop_unit_index = prop->unit_index;
            /* object slot for the prop's unit: data[3*idx + 2] is its object-data pointer */
            if ( ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop_unit_index)->datum)->unit.player_index != -1 )
            {
                actor_datum *actor = DATUM_GET(actor_data, actor_datum, prop->owner_actor_index);
                int object_index = actor->meta.swarm ? actor->meta.swarm_unit_index : actor->meta.unit_index;

                uint8_t engaging = 0;
                /* the unit's definition tag: unit-block flag 0x80000 marks "can endanger the player" */
                unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
                unit_definition *object_tag = TAG_GET(unit_definition, object_data->definition_index);
                if ( (object_tag->unit.flags & (1u << _unit_is_inconsequential)) != 0 )
                    engaging = prop->distance > 4.0f;

                if ( must_be_attacking
                     && !actor->control.fire_state            /* not suppressed */
                     && actor->state.action != actor_action_charge             /* not in the excluded state */
                     && prop->distance > 15.0f )
                    engaging = 1;

                if ( !engaging )
                {
                    int16_t state = prop->state;

                    if ( state < _prop_state_uninspected_orphan || state > _prop_state_inspected_orphan )
                    {
                        int last_seen_time = prop->last_visible_time;
                        if ( last_seen_time != -1 && last_seen_time + 90 >= current_time )
                            return 1;       /* recently had eyes on the player */
                    }

                    if ( (state < _prop_state_uninspected_orphan || state > _prop_state_inspected_orphan) && prop->distance < 4.0f )
                        return 1;           /* extremely close */

                    if ( actor->target.target_prop_index == iterator.index )   /* actor is targeting this prop */
                    {
                        if ( state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged )
                            return 1;       /* actively searching/closing */

                        if ( state >= _prop_state_uninspected_orphan && state <= _prop_state_inspected_orphan )
                        {
                            if ( prop->definitely_located )
                                return 1;

                            if ( state == _prop_state_uninspected_orphan && prop->distance < 12.0f )
                            {
                                prop_datum *target = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop->___u3.parent_prop_index);
                                float dz = prop->body_position.n[2] - target->body_position.n[2];
                                float dx = prop->body_position.n[0] - target->body_position.n[0];
                                float dy = prop->body_position.n[1] - target->body_position.n[1];
                                if ( (dy * dy) + ((dx * dx) + (dz * dz)) < 16.0f )
                                    return 1;   /* target is within 4 world units */
                            }
                        }
                    }
                }
            }
        }

        prop = data_iterator_next(&iterator);
        if ( !prop )
            return 0;
    }
}
