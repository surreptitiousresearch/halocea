/* actor_perception_desire_prop @0x837D6490 — decide whether an actor wants to keep a prop for a unit it has
 * perceived: combines target-state, whether the unit is a player/enemy/dead, distance, corpse age, encounter
 * strength and the actor's awareness. Returns the keep/desire decision; *optional_reference receives a
 * secondary "interested but not desiring" flag.
 *
 * Deviation: the database prototype has 13 parameters; Hex-Rays invented phantom stack params (a14..a38) from
 * the GPR slots reserved by the two float args (suicide_radius/distance_squared in f1/f2). The phantoms used
 * in the body map to real params: a30 = dead_ticks, a36 = required_ticks, a38 = optional_reference. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern int16_t actor_action_class(uint16_t actor_index);

uint8_t actor_perception_desire_prop(uint16_t actor_index, int16_t desired_target_state,
                                             int unit_index, int unit_actor_index, uint8_t in_use,
                                             uint8_t player, uint8_t enemy, uint8_t dead,
                                             int16_t dead_ticks, float suicide_radius, float distance_squared,
                                             int16_t required_ticks, uint8_t *optional_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_datum *unit_actor = (unit_actor_index == -1) ? 0 : DATUM_GET(actor_data, actor_datum, unit_actor_index);
    int enemy_flag = enemy;
    uint8_t secondary = 0;
    uint8_t desire;

    if ( (!enemy || dead) && desired_target_state >= _prop_state_uninspected_orphan
                         && desired_target_state <= _prop_state_inspected_orphan )   /* an orphan (remembered) state */
    {
        desire = 0;
        goto done;
    }
    if ( player )
    {
        desire = 1;
        goto done;
    }
    if ( unit_actor && (!unit_actor->meta.active || unit_actor->meta.dormant) )  /* unit's actor inactive or dormant */
    {
        desire = 0;
        goto done;
    }
    if ( desired_target_state == -1 && (in_use || required_ticks > 0) )
    {
        desire = 1;
        goto done;
    }
    if ( distance_squared > 1600.0f )
        goto reject;

    if ( dead )
    {
        char too_close_corpse = 0;
        desire = 1;
        if ( actor->meta.encounter_index != -1 )  /* actor.encounter_index */
        {
            int corpse_ignore_time = actor->emotions.corpse_ignore_time;
            encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum,
                                                            actor->meta.encounter_index);
            if ( encounter->corpse_ignore_time > corpse_ignore_time )
                corpse_ignore_time = encounter->corpse_ignore_time;
            if ( corpse_ignore_time != -1 )
            {
                int time_of_death = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum))->unit.time_of_death;
                if ( time_of_death == -1 || time_of_death < corpse_ignore_time )
                    desire = 0;
            }
            if ( encounter->enemy_visible || encounter->enemy_alive || (too_close_corpse = 1, encounter->stand_down) )
                too_close_corpse = 0;
        }
        if ( desire )
        {
            if ( too_close_corpse )
            {
                desire = distance_squared < 225.0f;
            }
            else if ( suicide_radius <= 0.0f )
            {
                if ( enemy_flag && dead_ticks > 150 )
                {
                    desire = 0;
                }
                else if ( actor_action_class(actor_index) <= 1 )
                {
                    float threshold = 16.0f;
                    if ( !enemy_flag && actor->state.mode < _actor_mode_combat )
                        threshold = 64.0f;
                    if ( distance_squared >= threshold )
                        goto reject;
                    desire = 1;
                }
                else
                {
                    desire = 0;
                }
            }
            else
            {
                desire = 1;
            }
        }
    }
    else
    {
        float secondary_threshold;
        if ( enemy )
        {
            desire = 1;
            secondary_threshold = 36.0f;
        }
        else
        {
            desire = distance_squared < 225.0f;
            if ( actor->state.combat_status >= _actor_combat_status_certain )
            {
                secondary = 1;
                goto done;
            }
            if ( actor->external_orders.pursuit_is_coordinator )
            {
                secondary = 0;
                goto done;
            }
            secondary_threshold = 16.0f;
        }
        secondary = distance_squared > secondary_threshold;
    }
    goto done;

reject:
    desire = 0;
done:
    if ( optional_reference )
        *optional_reference = secondary;
    return desire;
}
