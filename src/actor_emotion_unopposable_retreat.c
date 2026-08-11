/* actor_emotion_unopposable_retreat @0x837D8270 — per-tick "unopposable enemy" bookkeeping for one
 * actor: scans every prop (known unit) the actor is aware of, scoring each currently-opposed enemy's
 * threat level (0-4, from combat state/facing/visibility), or — for an enemy nobody is currently fighting
 * — checking whether a nearby ally already has one lined up for a retreat pairing. Builds up to 16
 * per-enemy-unit `actor_unopposable_enemy` scratch records (via actor_emotion_get_unopposable_enemy),
 * then runs each record through a hysteresis/escalation pass (using the actor's "unopposable retreat"
 * tag-definition fields) that raises `record.unopposable_type` to specific tiers (6/7/8/9) once a prop has
 * been visible long enough or the record's threat has stayed high/low long enough. Finally picks the
 * single highest-scoring record with a valid enemy actor and starts (or continues) a randomized retreat
 * cooldown against it.
 *
 * DEVIATION: see actor_emotion_get_unopposable_enemy.c's own header comment — its applied prototype is
 * wrong; both calls here pass the correct VALUES (a unit index, `&enemy_count`, 16, the scratch array,
 * an unused trailing arg) in the right positions, just displayed under the wrong types by the decompiler.
 *
 * `tag_base` fields at +616/618/620/624/628/632/634/648/652 are the actor_definition.unopposable.*
 * sub-block (reached via the standard TAG_INSTANCE(actor's own tag index) lookup), now typed.
 * The actor+932/936/940/944 "retreat pairing" fields are typed emotions.* members; both `actor`
 * (this function's own actor) and `other_actor` (a different actor found via a prop's owner) use them. */

#include "headers/actor_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/actor_unopposable_enemy.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_unopposable_danger_type.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern int prop_get_active_by_unit_index(int actor_index, int unit_index);
extern int16_t actor_emotion_get_unopposable_enemy(int unit_index, int16_t *enemy_count, int16_t maximum_enemy_count, actor_unopposable_enemy *enemies);
extern int game_time_get(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

extern int16_t actor_emotion_assess_unopposable_danger(int actor_index, int prop_index);
void actor_emotion_unopposable_retreat(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *props = (prop_datum *)prop_data->data;

    int16_t enemy_count = 0;
    /* "unopposable retreat" tag-definition base, via the actor's own tag index (actor+88). */
    actor_definition *tag_base = TAG_GET(actor_definition, actor->meta.definition_index);

    actor_unopposable_enemy enemies[16];

    prop_iterator iter;
    prop_iterator_new(&iter, actor_index);

    for (prop_datum *prop = prop_iterator_next(&iter); prop; prop = prop_iterator_next(&iter))
    {
        int prop_index = iter.index;
        /* DEVIATION: collapsed verbatim-inlined copy of zero-xref donor actor_emotion_assess_unopposable_danger@0x837D81D8 (field-for-field match at prop offsets 0x24/0xA4/0x74/0x12F/0x122/0x32). The donor subscripts prop_data with its FIRST parameter (clrlwi r11,r3,16 @0x837D81DC -> mulli r10,r11,0x138 @0x837D81E4; r4 is never read), and the inlined copy here feeds that same stride from r28 = iter.index, reloaded per iteration (lwz r28,var_238(r1) @0x837D82F8 -> clrlwi @0x837D82FC -> mulli 0x138 @0x837D8304) and stored as record->prop_index by stw r28,4(r11) @0x837D83D0 — NOT from actor_index, which lives in r25 (mr r25,r3 @0x837D82A0, used only for prop_get_active_by_unit_index @0x837D8468). So prop_index takes the first slot; the trailing arg is the donor's dead second parameter, kept to preserve the recovered prototype. */
        int16_t unopposable_type = actor_emotion_assess_unopposable_danger(prop_index, actor_index);

        if (unopposable_type <= 0)
        {
            /* Currently unopposed: if the prop itself is a live, close enemy, see whether a nearby ally
             * (via its own actor's retreat-pairing state) already has this unit lined up, and if so,
             * fold it into our own working set too. */
            if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged && !prop->enemy && prop->actor_index != -1
                    && prop->distance < 8.0f)
            {
                actor_datum *other_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
                if (other_actor->emotions.unopposable_retreat_timer)
                {
                    if (other_actor->emotions.unopposable_retreat_prop_index != -1)
                    {
                        int threshold = actor->emotions.unopposable_friend_ignore_time;
                        if (threshold == -1 || other_actor->emotions.unopposable_retreat_start_time >= threshold)
                        {
                            prop_datum *tracked_prop = &props[(uint16_t)other_actor->emotions.unopposable_retreat_prop_index];
                            int active_prop_index = prop_get_active_by_unit_index(actor_index,
                                    tracked_prop->unit_index);
                            if (active_prop_index != -1)
                            {
                                prop_datum *closest_prop = &props[(uint16_t)active_prop_index];
                                if (closest_prop->state >= _prop_state_becoming_unacknowledged && closest_prop->state <= _prop_state_acknowledged
                                        && closest_prop->unopposable_enemy)
                                {
                                    int16_t slot = actor_emotion_get_unopposable_enemy(
                                            tracked_prop->unit_index, &enemy_count, 16, enemies);
                                    if (slot != -1)
                                    {
                                        actor_unopposable_enemy *record = &enemies[slot];
                                        float distance_squared = tracked_prop->distance * tracked_prop->distance;
                                        record->friends_retreating++;
                                        if (distance_squared < record->closest_retreating_friend_dist_sq)
                                        {
                                            record->closest_retreating_friend_dist_sq = distance_squared;
                                            record->closest_retreating_friend_actor_index = prop->actor_index;
                                        }
                                        if (record->prop_index == -1)
                                        {
                                            record->prop_index = active_prop_index;
                                            record->prop = closest_prop;
                                            record->unit_index = closest_prop->unit_index;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            int16_t slot = actor_emotion_get_unopposable_enemy(prop->unit_index, &enemy_count, 16, enemies);
            if (slot != -1)
            {
                actor_unopposable_enemy *record = &enemies[slot];
                if (record->unopposable_type < unopposable_type)
                {
                    record->prop_index = prop_index;
                    record->unit_index = prop->unit_index;
                    record->prop = prop;
                    record->unopposable_type = unopposable_type;
                }
            }
        }
    }

    int enemy_total = enemy_count;
    for (int idx = 0; idx < enemy_total; idx++)
    {
        actor_unopposable_enemy *record = &enemies[idx];
        prop_datum *closest_prop = record->prop;

        int16_t hysteresis = tag_base->unopposable.trigger_unreachable_level;
        if (closest_prop->vehicle_gunner || closest_prop->dangerous_vehicle_driver)
            hysteresis = tag_base->unopposable.trigger_vehicle_level;

        if (closest_prop->player)
        {
            int16_t cap = tag_base->unopposable.trigger_player_level;
            if (cap > 0)
            {
                if (hysteresis <= cap)
                    cap = hysteresis;
                hysteresis = cap;
            }
        }

        unsigned char below_hysteresis = (hysteresis <= 0 || record->unopposable_type < hysteresis);
        unsigned char reset_hysteresis = 0;
        unsigned char escalate = 0;
        if (below_hysteresis)
        {
            if (closest_prop->player)
                reset_hysteresis = 1;
        }
        else
        {
            if (!closest_prop->player)
                reset_hysteresis = 1;
            else
                escalate = 1;
        }

        if (reset_hysteresis)
            closest_prop->unopposable_trigger_hysteresis = 22;

        if (closest_prop->unopposable_trigger_hysteresis > 0)
        {
            if (!closest_prop->unopposable_trigger_timer)
            {
                float upper = tag_base->unopposable.trigger_time_upper_bound;
                float lower = tag_base->unopposable.trigger_time_lower_bound;
                unsigned int *seed = get_global_random_seed_address();
                closest_prop->unopposable_trigger_threshold =
                        (int16_t)(real_seed_random_range(seed, lower, upper) * 30.0f);
            }
            int16_t previous_timer = closest_prop->unopposable_trigger_timer;
            closest_prop->unopposable_trigger_hysteresis--;
            closest_prop->unopposable_trigger_timer = previous_timer + 1;
        }

        if (closest_prop->visible_ticks >= 45 || record->unopposable_type >= _actor_unopposable_danger_damaging)
        {
            int16_t trigger_threshold = closest_prop->unopposable_trigger_threshold;
            if (trigger_threshold > 0 && closest_prop->unopposable_trigger_timer >= trigger_threshold
                    && record->unopposable_type <= _actor_unopposable_danger_time_triggered)
                record->unopposable_type = _actor_unopposable_danger_time_triggered;

            if (escalate && record->unopposable_type <= _actor_unopposable_danger_immediately_triggered)
                record->unopposable_type = _actor_unopposable_danger_immediately_triggered;

            int16_t casualties_threshold = tag_base->unopposable.casualties_trigger;
            if (casualties_threshold > 0
                    && closest_prop->unopposable_casualties_inflicted >= casualties_threshold
                    && record->unopposable_type <= _actor_unopposable_danger_friends_killed)
                record->unopposable_type = _actor_unopposable_danger_friends_killed;

            int16_t refresh_threshold = tag_base->unopposable.retreating_friends_trigger;
            if (refresh_threshold > 0 && record->friends_retreating >= refresh_threshold
                    && record->unopposable_type <= _actor_unopposable_danger_friends_retreating)
                record->unopposable_type = _actor_unopposable_danger_friends_retreating;
        }
    }

    if (actor->emotions.unopposable_retreat_timer <= 0)
    {
        /* No active retreat pairing: pick the highest-scoring record (above the floor of 5) with a
         * claimed prop and start a new randomized cooldown pairing against that prop. */
        int16_t best_score = _actor_unopposable_danger_retreat;
        int best_prop_index = -1;
        for (int idx = 0; idx < enemy_total; idx++)
        {
            actor_unopposable_enemy *record = &enemies[idx];
            if (record->unopposable_type > best_score && record->prop_index != -1)
            {
                best_score = record->unopposable_type;
                best_prop_index = record->prop_index;
            }
        }

        if (best_prop_index != -1)
        {
            float upper = tag_base->unopposable.retreat_time_upper_bound;
            float lower = tag_base->unopposable.retreat_time_lower_bound;
            unsigned int *seed = get_global_random_seed_address();
            float delay = real_seed_random_range(seed, lower, upper);
            actor->emotions.unopposable_retreat_prop_index = best_prop_index;
            actor->emotions.unopposable_retreat_timer = (int16_t)(delay * 30.0f);
            actor->emotions.unopposable_retreat_start_time = game_time_get();
        }
    }
    else
    {
        int16_t remaining = actor->emotions.unopposable_retreat_timer - 1;
        actor->emotions.unopposable_retreat_timer = remaining;
        if (!remaining)
            actor->emotions.unopposable_friend_ignore_time = game_time_get();
    }
}
