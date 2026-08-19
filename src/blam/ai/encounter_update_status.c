/* encounter_update_status @0x8370D478 — recompute one encounter's aggregate combat status for the frame.
 *
 * Walks every actor belonging to the encounter, tallying per-squad and per-platoon strength/count/swarm
 * totals plus the encounter-wide living/in-combat/fighting counts and the "enemy alive/visible" flags. From
 * those tallies it drives the encounter combat state machine (stand-down / post-combat / stay-active) and
 * finally normalises each squad's and platoon's strength fraction to [0,1).
 *
 * DEVIATION: `fsel f8,f8,f30` (f30=0) reduced to `t >= 0 ? t : 0`; `(_cntlzw(x) & 0x20) != 0`
 * reduced to `(x == 0)`. On de-escalation the "last active time" stamp is
 * encounter.corpse_ignore_time (ignore corpses older than battle end).
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/ai_globals.h"
#include "headers/actor_postcombat_type.h"
#include "headers/object_damage_flags.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_in_combat(int actor_index);
extern uint8_t actor_is_fighting(int actor_index);
extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern void encounter_post_combat(int encounter_index);
extern void encounter_stand_down(int encounter_index);
extern int game_time_get(void);

void encounter_update_status(int encounter_index)
{
    encounter_datum *encounter;
    uint8_t saw_traitor;
    uint8_t any_postcombat_pending;       /* external_orders.postcombat_type > actor_postcombat_none on any actor */
    uint8_t any_had_visible_enemy;        /* state.had_visible_enemy */
    uint8_t any_been_in_combat;           /* state.been_in_combat */
    int16_t i;
    int actor_index;
    int actor_iter;
    actor_datum *actor;
    int object_index;
    squad_datum *squad;
    platoon_datum *platoon;
    int16_t body_count;
    int16_t original_body_count;
    float strength;
    int platoon_local_index;
    int enemy_prop_index;
    prop_datum *enemy_prop;
    uint8_t fighting;
    ai_globals_t *globals;
    bool stay_active;

    encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    saw_traitor = 0;
    any_postcombat_pending = 0;
    any_had_visible_enemy = 0;
    any_been_in_combat = 0;
    encounter->current_strength_fraction = 0.0;
    encounter->enemy_alive = 0;
    encounter->enemy_visible = 0;
    encounter->current_fighting_count = 0;
    encounter->current_in_combat_count = 0;
    encounter->current_swarm_count = 0;
    encounter->current_count = 0;

    /* clear per-squad running totals */
    for (i = 0; i < encounter->squad_count; ++i)
    {
        squad = &squad_array[(int16_t)((uint16_t)encounter->squad_base + i)];
        squad->current_strength_fraction = 0.0;
        squad->current_swarm_count = 0;
        squad->current_count = 0;
    }

    /* clear per-platoon running totals */
    for (i = 0; i < encounter->platoon_count; ++i)
    {
        platoon = &platoon_array[(int16_t)((uint16_t)encounter->platoon_base + i)];
        platoon->current_strength_fraction = 0.0;
        platoon->current_swarm_count = 0;
        platoon->current_count = 0;
    }

    globals = ai_globals;
    /* DEVIATION: when !ai_initialized_for_map the decompiler read an uninitialized stack slot into
     * the iterator; the value is never used (the walk loop's guard is false), so it is left unset. */
    actor_iter = -1;
    if (ai_globals->ai_initialized_for_map)
    {
        if (encounter_index == -1)
            actor_iter = ai_globals->first_encounterless_actor_index;
        else
            actor_iter = encounter->first_actor_index;
    }

    while (globals->ai_initialized_for_map)
    {
        actor_index = actor_iter;
        if (actor_iter == -1)
            break;
        actor = DATUM_GET(actor_data, actor_datum, actor_iter);
        actor_iter = actor->meta.next_actor_index;
        squad = &squad_array[(int16_t)((uint16_t)actor->meta.squad_index + (uint16_t)encounter->squad_base)];
        object_index = actor->meta.unit_index;
        if (object_index == -1)
        {
            body_count = (uint16_t)actor->meta.swarm_unit_count;
            original_body_count = actor->meta.swarm_original_unit_count;
            strength = (float)body_count / (float)original_body_count;
        }
        else
        {
            body_count = 1;
            strength = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum))->object.body_vitality;
        }

        platoon_local_index = (uint16_t)actor->meta.platoon_index;
        if (platoon_local_index != 0xFFFF)
        {
            platoon = &platoon_array[(int16_t)((uint16_t)encounter->platoon_base + platoon_local_index)];
            platoon->current_count += body_count;
            platoon->current_strength_fraction = platoon->current_strength_fraction + strength;
            platoon->current_swarm_count = actor->meta.swarm * body_count + platoon->current_swarm_count;
        }

        squad->current_count += body_count;
        squad->current_strength_fraction = squad->current_strength_fraction + strength;
        squad->current_swarm_count = actor->meta.swarm * body_count + squad->current_swarm_count;

        encounter->current_count += body_count;
        encounter->current_swarm_count = actor->meta.swarm * body_count + encounter->current_swarm_count;
        encounter->current_in_combat_count += actor_in_combat(actor_index) * body_count;
        fighting = actor_is_fighting(actor_index);
        encounter->current_strength_fraction = encounter->current_strength_fraction + strength;
        encounter->current_fighting_count = fighting * body_count + encounter->current_fighting_count;

        enemy_prop_index = actor->target.target_prop_index;
        if (enemy_prop_index != -1)
        {
            encounter->enemy_target = 1;
            enemy_prop = DATUM_GET(prop_data, prop_datum, enemy_prop_index);
            if (!game_team_is_ally((uint16_t)actor->meta.team_index, (uint16_t)enemy_prop->team_index))
                saw_traitor = 1;
            actor_in_combat(actor_index);
            if (actor->state.had_visible_enemy)
                any_had_visible_enemy = 1;
            if (actor->state.been_in_combat)
                any_been_in_combat = 1;
            if (actor->state.combat_status >= _actor_combat_status_visible)            /* fully aware */
            {
                encounter->enemy_visible = 1;
                goto mark_enemy_alive;
            }
            {
                int prop_state = enemy_prop->state;
                bool target_perceivable;
                if (prop_state < _prop_state_becoming_unacknowledged || prop_state > _prop_state_acknowledged)
                    target_perceivable =
                        ((((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, enemy_prop->unit_index)->datum))->object.damage_flags & (1u << _object_dead_bit)) == 0;
                else
                    target_perceivable = enemy_prop->dead == 0;
                if (target_perceivable)
                {
mark_enemy_alive:
                    encounter->enemy_alive = 1;
                }
            }
        }
        globals = ai_globals;
        if (actor->external_orders.postcombat_type > actor_postcombat_none)
            any_postcombat_pending = 1;
    }

    /* CAVEAT: as-shipped — the binary's only store to enemy_traitor is stb r26,0x46(r31)
     * @0x8370D88C with r26=0 (li r26,0 @0x8370D4A8), guarded on saw_traitor; the flag is
     * never written 1 anywhere in this function. */
    if (saw_traitor)
        encounter->enemy_traitor = 0;

    stay_active = 0;
    {
        int enemy_visible_timer = encounter->enemy_visible_timer;
        int enemy_alive_timer = encounter->enemy_alive_timer;
        if (encounter->enemy_visible
            || (enemy_visible_timer != -1) && enemy_visible_timer < 60
            || (encounter->enemy_alive || (enemy_alive_timer != -1) && enemy_alive_timer < 60)
               && enemy_visible_timer != -1 && enemy_visible_timer < 450)
        {
            stay_active = 1;
        }
    }

    if (stay_active)
    {
        encounter->stand_down = 0;
        encounter->post_combat = 0;
        goto normalize;
    }

    if (!encounter->stand_down)
    {
        if (encounter->post_combat)
        {
            int post_combat_delay_timer = (uint16_t)encounter->post_combat_delay_timer;
            encounter->post_combat_delay = (any_postcombat_pending == 0); /* DEVIATION: (_cntlzw(x)&0x20)!=0 == (x==0) */
            if (post_combat_delay_timer)
                goto normalize;
        }
        else if (any_had_visible_enemy && any_been_in_combat)
        {
            encounter_post_combat(encounter_index);
            goto normalize;
        }
        encounter_stand_down(encounter_index);
        goto normalize;
    }

    /* encounter was active and is now de-escalating */
    {
        int16_t living = (uint16_t)encounter->current_count;
        encounter->post_combat = 0;
        encounter->prebattle_living_count = living;
        int now = game_time_get();
        int has_enemy_target = encounter->enemy_target;
        encounter->corpse_ignore_time = now;
        encounter->enemies_defeated = 0;
        if (!has_enemy_target)
        {
            encounter->enemy_visible_timer = -1;
            encounter->enemy_alive_timer = -1;
        }
    }

normalize:
    /* normalise the encounter strength fraction, then each squad's and platoon's, clamped at 0 */
    {
        int16_t original_count = encounter->original_count;
        if (original_count > 0)
        {
            float t = (encounter->current_strength_fraction / (float)original_count) - 0.001f;
            encounter->current_strength_fraction = t >= 0.0f ? t : 0.0f; /* fsel f8,f8,f30 with f30=0 */
        }
    }
    for (i = 0; i < encounter->squad_count; ++i)
    {
        squad = &squad_array[(int16_t)((uint16_t)encounter->squad_base + i)];
        float t = (squad->current_strength_fraction / (float)squad->original_count) - 0.001f;
        squad->current_strength_fraction = t >= 0.0f ? t : 0.0f;
    }
    for (i = 0; i < encounter->platoon_count; ++i)
    {
        platoon = &platoon_array[(int16_t)((uint16_t)encounter->platoon_base + i)];
        float t = (platoon->current_strength_fraction / (float)platoon->original_count) - 0.001f;
        platoon->current_strength_fraction = t >= 0.0f ? t : 0.0f;
    }
    encounter->status_dirty = 0;
}
