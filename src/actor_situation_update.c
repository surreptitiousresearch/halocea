/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* actor_situation_update @ 0x837D7530 — rebuild the actor's tactical "situation" tally for this frame by
 * iterating its props (its knowledge of nearby units). The counter block at actor+492 (123 bytes) is cleared
 * and then filled: enemy props bump threat counters bucketed by a computed threat category (threat_category
 * below: damaging me > facing-and-close > shooting > visible, etc.), while ally props bump support counters by
 * the ally's actor type (player=6, AI=meta.type, unknown=14). Each prop also feeds the best-target selection: the
 * prop with the highest target_weight becomes the active prop (actor+624 -> target.target_prop_index). When it
 * changes, the old and new props' cached target_weight is recomputed. Finally the per-target and combat-status
 * summaries are refreshed.
 *
 * Counter block (byte offsets relative to actor base -> actor_situation members):
 *   +492 -> known_enemies       +493 -> visible_reachable_enemies   +494..+502 -> specific_threats[9]
 *   +503..+511 -> cumulative_threats[9] (every category a prop qualifies for; specific_threats gets only its top)
 *   +512 -> area_friends        +513 -> area_fighting_friends       +514 -> area_fire_support_friends
 *   +515.. -> area_friends_by_type[16]                              +531.. -> area_fighting_friends_by_type[16]
 *   +547 -> visible_friends     +548 -> visible_fighting_friends    +549../+565.. -> the visible _by_type pair
 *   +581 -> close_friends       +582 -> close_fighting_friends      +583../+599.. -> the close _by_type pair
 * Layout is src/headers/actor_situation.h (actor_datum +0x1EC, 123 bytes); all accesses below are by member. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include <string.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/actor_action.h"
#include "headers/actor_target_type.h"
#include "headers/actor_threat_type.h"
#include "headers/actor_type.h"
#include "headers/ai_line_of_sight.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);


extern float actor_compute_prop_target_weight(int actor_index, int prop_index);
extern void actor_situation_update_target_status(uint16_t actor_index);
extern void actor_situation_combat_status_update(uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

void actor_situation_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* suppress the "facing & very close" threat bump while charging or when actor+888 -> emotions.berserk is set */
    char suppress_close_threat = (actor->emotions.berserk || actor->state.action == actor_action_charge) ? 1 : 0;

    int   best_index  = -1;
    float best_weight = 0.0f;

    memset(&actor->situation, 0, sizeof(actor->situation));

    prop_iterator iterator;
    prop_iterator_new(&iterator, actor_index);

    for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
    {
        int state = prop->state;
        if ( state < _prop_state_becoming_unacknowledged || state > _prop_state_acknowledged || prop->dead )
            goto select_target;

        if ( prop->enemy )
        {
            int threat_category = actor_threat_none;
            int visibility = prop->visibility;

            ++actor->situation.known_enemies;
            if ( visibility >= 2 && !prop->unreachable_ticks )
                ++actor->situation.visible_reachable_enemies;

            char detail = 0;
            if ( visibility >= 2 )
            {
                threat_category = actor_threat_visible;
                ++actor->situation.cumulative_threats[actor_threat_visible];
                detail = 1;
            }
            else if ( prop->shooting && prop->line_of_sight == _ai_line_of_sight_clear )
            {
                detail = 1;
            }

            if ( detail )
            {
                if ( prop->currently_damaging_me )
                {
                    ++actor->situation.cumulative_threats[actor_threat_damaging_me];
                    if ( threat_category <= actor_threat_damaging_me ) threat_category = actor_threat_damaging_me;
                }
                if ( prop->shooting )
                {
                    ++actor->situation.cumulative_threats[actor_threat_shooting];
                    if ( threat_category <= actor_threat_shooting ) threat_category = actor_threat_shooting;
                }
                if ( prop->quantized_facing <= 2 )
                {
                    if ( visibility >= 2 )
                    {
                        ++actor->situation.cumulative_threats[actor_threat_visible_facing_me];
                        if ( threat_category <= actor_threat_visible_facing_me ) threat_category = actor_threat_visible_facing_me;
                        if ( !suppress_close_threat && prop->distance < 2.0f )
                        {
                            ++actor->situation.cumulative_threats[actor_threat_extremely_close_to_me];
                            if ( threat_category <= actor_threat_extremely_close_to_me ) threat_category = actor_threat_extremely_close_to_me;
                        }
                    }
                    if ( prop->quantized_facing <= 1 )
                    {
                        if ( prop->shooting )
                        {
                            ++actor->situation.cumulative_threats[actor_threat_shooting_near_me];
                            if ( threat_category <= actor_threat_shooting_near_me ) threat_category = actor_threat_shooting_near_me;
                        }
                        if ( prop->quantized_facing <= 0 )
                        {
                            if ( visibility >= 2 )
                            {
                                ++actor->situation.cumulative_threats[actor_threat_visible_aiming_at_me];
                                if ( threat_category <= actor_threat_visible_aiming_at_me ) threat_category = actor_threat_visible_aiming_at_me;
                            }
                            if ( prop->shooting )
                            {
                                ++actor->situation.cumulative_threats[actor_threat_shooting_at_me];
                                if ( threat_category <= actor_threat_shooting_at_me ) threat_category = actor_threat_shooting_at_me;
                            }
                        }
                    }
                }
            }

            ++actor->situation.specific_threats[threat_category];
        }
        else
        {
            /* ally prop: bucket by the ally unit's actor type */
            unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum);
            actor_datum *controlling_actor = (unit_object->unit.actor_index == -1)
                                    ? NULL
                                    : DATA_ARRAY_ELEMENT(actor_data, actor_datum, unit_object->unit.actor_index);

            int ally_type;
            if ( unit_object->unit.player_index == -1 )
                ally_type = controlling_actor ? controlling_actor->meta.type : _actor_type_none;
            else
                ally_type = _actor_type_player;

            float distance = prop->distance;
            char near_or_same_target = 0, has_los = 0, very_close = 0;

            int my_target = actor->target.target_prop_index;   /* actor's active prop */
            int ally_target;
            if ( distance < 8.0f
              || (prop->fighting
                  && controlling_actor
                  && my_target != -1
                  && (ally_target = controlling_actor->target.target_prop_index) != -1
                  /* DEVIATION: both prop handles address prop_data by their LOW WORD only —
                   * 837D78E8/837D78EC clrlwi r10,r10,16 / clrlwi r4,r9,16 feed the mulli 0x138.
                   * The decompiler dropped the mask and subscripted the full salted handle. */
                  && DATA_ARRAY_ELEMENT(prop_data, prop_datum, my_target)->unit_index
                       == DATA_ARRAY_ELEMENT(prop_data, prop_datum, ally_target)->unit_index) )
                near_or_same_target = 1;

            if ( prop->line_of_sight == _ai_line_of_sight_clear || prop->line_of_sight == _ai_line_of_sight_occluded )
            {
                has_los = 1;
                very_close = distance < 3.0f;
            }

            if ( near_or_same_target )
            {
                ++actor->situation.area_friends;
                if ( prop->fighting )                      ++actor->situation.area_fighting_friends;
                if ( prop->fighting && prop->vehicle_gunner ) ++actor->situation.area_fire_support_friends;
                ++actor->situation.area_friends_by_type[ally_type];
                if ( prop->fighting ) ++actor->situation.area_fighting_friends_by_type[ally_type];
            }
            if ( has_los )
            {
                ++actor->situation.visible_friends;
                if ( prop->fighting ) ++actor->situation.visible_fighting_friends;
                ++actor->situation.visible_friends_by_type[ally_type];
                if ( prop->fighting ) ++actor->situation.visible_fighting_friends_by_type[ally_type];
            }
            if ( very_close )
            {
                ++actor->situation.close_friends;
                if ( prop->fighting ) ++actor->situation.close_fighting_friends;
                ++actor->situation.close_friends_by_type[ally_type];
                if ( prop->fighting ) ++actor->situation.close_fighting_friends_by_type[ally_type];
            }
        }

select_target:
        if ( prop->target_weight > best_weight )
        {
            best_index  = iterator.index;
            best_weight = prop->target_weight;
        }
    }

    int prev_target = actor->target.target_prop_index;
    if ( best_index != prev_target )
    {
        actor->target.target_prop_index   = best_index;
        actor->target.target_type = actor_target_none;
        actor->target.target_last_visible_time = -1;
        /* DEVIATION: 837D7AD0 clrlwi r10,r4,16 and 837D7AF8 clrlwi r10,r24,16 precede each mulli 0x138 —
         * the stored-to slot is the handle's low word. The weight-recompute ARGUMENT stays the raw
         * handle (r4 = untouched 0x270 load at 837D7AE4 / reloaded at 837D7B00), so only the subscript
         * masks; the decompiler had dropped the mask from both. */
        if ( prev_target != -1 )
            DATA_ARRAY_ELEMENT(prop_data, prop_datum, prev_target)->target_weight =
                actor_compute_prop_target_weight(actor_index, prev_target);
        if ( best_index != -1 )
            DATA_ARRAY_ELEMENT(prop_data, prop_datum, best_index)->target_weight =
                actor_compute_prop_target_weight(actor_index, actor->target.target_prop_index);
    }

    actor_situation_update_target_status(actor_index);
    actor_situation_combat_status_update(actor_index);
}
