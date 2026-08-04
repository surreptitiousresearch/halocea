/* encounter_update_status @0x8370D478 — recompute one encounter's aggregate combat status for the frame.
 *
 * Walks every actor belonging to the encounter, tallying per-squad and per-platoon strength/count/swarm
 * totals plus the encounter-wide living/in-combat/fighting counts and the "enemy alive/visible" flags. From
 * those tallies it drives the encounter combat state machine (stand-down / post-combat / stay-active) and
 * finally normalises each squad's and platoon's strength fraction to [0,1).
 *
 * Reconstructed from the decompiler. PPC idioms reduced inline: the `fsel f8,f8,f30` clamp becomes
 * `t >= 0 ? t : 0`, and `(_cntlzw(x) & 0x20) != 0` becomes `(x == 0)`.
 * 2026-07-14 fully typed (encounter_datum/actor_datum/prop_datum/unit_datum); on de-escalation the
 * "last active time" stamp is in fact encounter.corpse_ignore_time (ignore corpses older than battle end).
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


extern uint8_t actor_in_combat(uint16_t actor_index);
extern uint8_t actor_is_fighting(uint16_t actor_index);
extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern void encounter_post_combat(int encounter_index);
extern void encounter_stand_down(int encounter_index);
extern int game_time_get(void);

void encounter_update_status(int encounter_index)
{
    encounter_datum *encounter;
    char saw_traitor;                     /* v4 */
    unsigned __int8 any_postcombat_pending; /* v5 — external_orders.postcombat_type > actor_postcombat_none on any actor */
    char any_had_visible_enemy;           /* v6 — state.had_visible_enemy */
    char any_been_in_combat;              /* v8 — state.been_in_combat */
    __int16 squad_count;                  /* v7 */
    __int16 i;
    int actor_index;                      /* v19 */
    int actor_iter;                       /* first_encounterless_actor_index */
    actor_datum *actor;                   /* v20 */
    int object_index;
    squad_datum *squad;
    platoon_datum *platoon;
    short body_count;                     /* v23 */
    short original_body_count;            /* v25 */
    float strength;                       /* v24 */
    int platoon_local_index;              /* v26 */
    int enemy_prop_index;                 /* v38 */
    prop_datum *enemy_prop;               /* v40 */
    unsigned __int8 fighting;
    ai_globals_t *globals;
    bool stay_active;                     /* v43 != 0 path */
    int recompute_clamp_count;            /* v50 high/low scratch */

    encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    saw_traitor = 0;
    any_postcombat_pending = 0;
    any_had_visible_enemy = 0;
    squad_count = (unsigned __int16)encounter->squad_count;
    encounter->current_strength_fraction = 0.0;
    any_been_in_combat = 0;
    encounter->enemy_alive = 0;
    encounter->enemy_visible = 0;
    encounter->current_fighting_count = 0;
    encounter->current_in_combat_count = 0;
    encounter->current_swarm_count = 0;
    encounter->current_count = 0;

    /* clear per-squad running totals */
    if (squad_count > 0)
    {
        __int16 n = 0;
        i = 0;
        do
        {
            int squad_index = (__int16)((unsigned __int16)encounter->squad_base + i);
            i = ++n;
            squad = &squad_array[squad_index];
            squad->current_strength_fraction = 0.0;
            squad->current_swarm_count = 0;
            squad->current_count = 0;
        } while (n < encounter->squad_count);
    }

    /* clear per-platoon running totals */
    if (encounter->platoon_count > 0)
    {
        __int16 n = 0;
        i = 0;
        do
        {
            int platoon_index = (__int16)((unsigned __int16)encounter->platoon_base + i);
            i = ++n;
            platoon = &platoon_array[platoon_index];
            platoon->current_strength_fraction = 0.0;
            platoon->current_swarm_count = 0;
            platoon->current_count = 0;
        } while (n < encounter->platoon_count);
    }

    globals = ai_globals;
    if (ai_globals->ai_initialized_for_map)
    {
        if (encounter_index == -1)
            actor_iter = ai_globals->first_encounterless_actor_index;
        else
            actor_iter = encounter->first_actor_index;
    }
    else
    {
        actor_iter = recompute_clamp_count; /* uninitialized v65 — preserved decompiler artifact */
    }

    while (globals->ai_initialized_for_map)
    {
        actor_index = actor_iter;
        if (actor_iter == -1)
            break;
        actor = DATUM_GET(actor_data, actor_datum, actor_iter);
        actor_iter = actor->meta.next_actor_index;
        squad = &squad_array[(__int16)((unsigned __int16)actor->meta.squad_index + (unsigned __int16)encounter->squad_base)];
        object_index = actor->meta.unit_index;
        if (object_index == -1)
        {
            body_count = (unsigned __int16)actor->meta.swarm_unit_count;
            original_body_count = actor->meta.swarm_original_unit_count;
            strength = (float)body_count / (float)original_body_count;
        }
        else
        {
            body_count = 1;
            strength = (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum))->object.body_vitality;
        }

        platoon_local_index = (unsigned __int16)actor->meta.platoon_index;
        if (platoon_local_index != 0xFFFF)
        {
            platoon = &platoon_array[(__int16)((unsigned __int16)encounter->platoon_base + platoon_local_index)];
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
            if (!game_team_is_ally((unsigned __int16)actor->meta.team_index, (unsigned __int16)enemy_prop->team_index))
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
            int post_combat_delay_timer = (unsigned __int16)encounter->post_combat_delay_timer;
            encounter->post_combat_delay = (any_postcombat_pending == 0); /* (_cntlzw(v5)&0x20)!=0 == (v5==0) */
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
        __int16 living = (unsigned __int16)encounter->current_count;
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
        __int16 original_count = encounter->original_count;
        if (original_count > 0)
        {
            float t = (encounter->current_strength_fraction / (float)original_count) - (float)0.001;
            encounter->current_strength_fraction = t >= 0.0f ? t : 0.0f; /* fsel f8,f8,f30 with f30=0 */
        }
    }
    if (encounter->squad_count > 0)
    {
        __int16 n = 0;
        __int16 idx = 0;
        do
        {
            int squad_index = (__int16)((unsigned __int16)encounter->squad_base + idx);
            idx = ++n;
            squad = &squad_array[squad_index];
            float t = (squad->current_strength_fraction / (float)squad->original_count) - (float)0.001;
            squad->current_strength_fraction = t >= 0.0f ? t : 0.0f;
        } while (n < encounter->squad_count);
    }
    if (encounter->platoon_count > 0)
    {
        __int16 n = 0;
        __int16 idx = 0;
        do
        {
            int platoon_index = (__int16)((unsigned __int16)encounter->platoon_base + idx);
            idx = ++n;
            platoon = &platoon_array[platoon_index];
            float t = (platoon->current_strength_fraction / (float)platoon->original_count) - (float)0.001;
            platoon->current_strength_fraction = t >= 0.0f ? t : 0.0f;
        } while (n < encounter->platoon_count);
    }
    encounter->status_dirty = 0;
}
