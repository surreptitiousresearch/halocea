/* object_damage_update @ 0x836B4160 — per-tick damage/shield bookkeeping for a unit object. Applies any
 * pending falling/distance/aux damage (damage_flags), recharges or decays shield_vitality toward full
 * using the collision model's runtime_shield_recharge_velocity, difficulty-scaled, gated by the HCEX
 * "recharge on melee" mod (hcex_recharge_ticks per player). Drives the HUD shield meter, spawns the
 * shield_recharging_effect and re-enables shield region permutations when recovery starts, and advances
 * the shield/body damage-decay timers. Finally, for a dead biped simulated locally it sets the
 * "needs sync" bit (0x4000000) when (shield_stun_ticks > 0) diverges from the network baseline.
 *
 * Deviations from the old transcription (all DB-verified): the tag read via definition[31] is the
 * collision_model reference (not a "unit definition"); +448 is
 * damage_resistance.runtime_shield_recharge_velocity; +436 is shield_recharging_effect.index; the final
 * check compares shield_stun_ticks>0 against biped.baseline.shield_stun_ticks_greater_than_zero (the old
 * "destroyed-region mask" description was wrong). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_falling_damage.h"
#include "headers/damage_data.h"
#include "headers/damage_data_flags.h"
#include "headers/real_rgb_color.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/player_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_difficulty_value.h"
#include "headers/game_time_constants.h"

struct effect_vector_field;


#include "headers/effect_vector_field.h"
extern int player_index_from_unit_index(int unit_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void object_permutation_shield_regions(int object_index, uint8_t active);
extern void hud_tick_shield(int player_index, float amount);
extern uint8_t game_engine_running(void);
extern void  *memset(void *dst, int value, unsigned int count);

void object_damage_update(int object_index)
{
    object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *definition = TAG_GET(_object_definition, unit->definition_index);
    int collision_model_index = definition->collision_model.index;
    collision_model *collision =
        (collision_model_index == -1) ? 0 : TAG_GET(collision_model, collision_model_index);

    /* HCEX "recharge on melee" gate: shields only regen while the player's melee-recharge budget lasts. */
    unsigned char allow_recharge = 1;
    __int16 player_index;
    if ( player_index_from_unit_index(object_index) == -1 )
        player_index = -1;
    else
        player_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
            player_index_from_unit_index(object_index))->local_player_index;
    if ( player_index != -1 )
    {
        if ( hcex_recharge_on_melee )
        {
            int ticks = hcex_recharge_ticks[player_index];
            if ( ticks <= 0 )
                allow_recharge = 0;
            else
                hcex_recharge_ticks[player_index] = ticks - 1;
        }
        else
        {
            hcex_recharge_ticks[player_index] = 0;
        }
    }

    if ( collision )
    {
        __int16 damage_flags = unit->object.damage_flags;
        if ( (damage_flags & (1u << _object_die_act_of_god_no_statistics_bit)) != 0 || (damage_flags & (1u << _object_die_act_of_god_bit)) != 0 || (damage_flags & (1u << _object_die_act_of_god_silent_bit)) != 0 )
        {
            if ( (damage_flags & (1u << _object_dead_bit)) == 0 )
            {
                int falling_damage_effect = ((game_globals_falling_damage *)global_game_globals->falling_damage.address)->falling_damage.index;
                if ( falling_damage_effect != -1 )
                {
                    damage_data damage;

                    /* Faithful reproduction of the binary's struct prelude: 10 qwords of {10, 0}
                     * followed by a trailing int = 0, before the explicit field overrides below. */
                    char *cursor = (char *)&damage - 8;
                    for ( int i = 10; i; --i )
                    {
                        cursor += 8;
                        *(long long *)cursor = 0xA00000000LL;
                    }
                    ((int *)cursor)[2] = 0;

                    damage.multiplier = 1.0f;
                    unsigned int flags = damage.flags | (1u << _damage_kill_instantly_bit);
                    damage.scale = 1.0f;
                    damage.definition_index = falling_damage_effect;
                    damage.material_type = -1;
                    memset(&damage.owner_player_index, 255, 10);
                    damage.location.cluster_index = -1;
                    damage.flags |= (1u << _damage_kill_instantly_bit);
                    if ( (unit->object.damage_flags & (1u << _object_die_act_of_god_silent_bit)) != 0 )
                    {
                        flags |= (1u << _damage_silent_bit);
                        damage.flags = flags;
                    }
                    if ( (unit->object.damage_flags & (1u << _object_die_act_of_god_no_statistics_bit)) != 0 )
                        damage.flags = flags | (1u << _damage_no_statistics_bit);

                    object_cause_damage(&damage, object_index, -1, -1, -1, (const real_vector3d *)0);
                }
            }
            unit->object.damage_flags &= ~((1u << _object_die_act_of_god_bit)
                | (1u << _object_die_act_of_god_silent_bit)
                | (1u << _object_die_act_of_god_no_statistics_bit));
        }

        float max_shield = unit->object.maximum_shield_vitality;
        __int16 cleared_flags = unit->object.damage_flags & ~(1u << _object_shield_charging_bit);
        unit->object.damage_flags = cleared_flags;
        if ( max_shield > 0.0f && (cleared_flags & (1u << _object_dead_bit)) == 0 )
        {
            if ( (unit->object.damage_flags & (1u << _object_shield_over_charging_bit)) != 0 )
            {
                /* Shield was just damaged: count up the recharge-delay grace period. */
                if ( allow_recharge )
                {
                    float delay = unit->object.shield_vitality + SECONDS_PER_TICK;
                    unit->object.shield_vitality = unit->object.shield_vitality + SECONDS_PER_TICK;
                    if ( delay < 3.0f )
                    {
                        unit->object.damage_flags |= (1u << _object_shield_charging_bit);
                    }
                    else
                    {
                        unit->object.shield_vitality = 3.0f;
                        unit->object.damage_flags &= ~(1u << _object_shield_over_charging_bit);
                    }
                }
            }
            else if ( unit->object.shield_vitality > 1.0f && game_engine_running() )
            {
                /* Overcharged (>1): bleed back to full, ticking the HUD meter. */
                int hud_player = player_index_from_unit_index(object_index);
                float overcharge = unit->object.shield_vitality - 1.0f;
                if ( overcharge >= 0.00074074074f )
                {
                    unit->object.shield_vitality = unit->object.shield_vitality - 0.00074074074f;
                    hud_tick_shield(hud_player, 0.00074074074f);
                }
                else
                {
                    unit->object.shield_vitality = 1.0f;
                    hud_tick_shield(hud_player, overcharge);
                }
            }
            else if ( unit->object.shield_vitality < 1.0f )
            {
                /* Below full: wait out the recharge delay, then regenerate. */
                __int16 recharge_delay = unit->object.shield_stun_ticks;
                if ( recharge_delay )
                {
                    int network_sim = unit->object.datum_role;
                    if ( network_sim == 3 || !network_sim )
                        unit->object.shield_stun_ticks = recharge_delay - 1;
                }
                else
                {
                    float recharge_rate = collision->resistance.runtime_shield_recharge_velocity;
                    float regen = game_difficulty_get_team_value(_game_difficulty_enemy_recharge_scale, unit->object.owner_team_index) * recharge_rate;
                    if ( (unit->object.damage_flags & (1u << _object_shield_depleted_bit)) != 0 )
                    {
                        /* DEVIATION: decompiler left color/impulse_field args uninitialized; passed null. */
                        effect_new_from_object(collision->resistance.shield_recharging_effect.index,
                            object_index, object_index,
                            -1, 0.0f, 0.0f, (const real_rgb_color *)0, (const struct effect_vector_field *)0);
                        unit->object.damage_flags &= ~(1u << _object_shield_depleted_bit);
                        object_permutation_shield_regions(object_index, 1u);
                    }
                    if ( allow_recharge )
                    {
                        __int16 flags = unit->object.damage_flags;
                        float new_shield = unit->object.shield_vitality + regen;
                        unit->object.shield_vitality = unit->object.shield_vitality + regen;
                        __int16 recharging = flags | (1u << _object_shield_charging_bit);
                        unit->object.damage_flags = recharging;
                        if ( new_shield > 1.0f )
                        {
                            unit->object.shield_vitality = 1.0f;
                            unit->object.damage_flags = recharging & ~(1u << _object_shield_charging_bit);
                        }
                    }
                }
            }
        }

        /* Advance the two recent-damage flash-timer pairs; clamp each component to >= 0. */
        int body_decay_elapsed = unit->object.body_damage_decay_timer;
        if ( body_decay_elapsed != -1 )
        {
            int next = body_decay_elapsed + 1;
            unit->object.body_damage_decay_timer = next;
            if ( next >= 0 )
                unit->object.current_body_damage = unit->object.current_body_damage - 0.016666668f;
            if ( next >= 60 )
                unit->object.recent_body_damage = unit->object.recent_body_damage - 0.016666668f;
            float a = unit->object.current_body_damage;
            float b = unit->object.recent_body_damage;
            a = (a >= 0.0f) ? a : 0.0f;
            b = (b >= 0.0f) ? b : 0.0f;
            unit->object.current_body_damage = a;
            unit->object.recent_body_damage = b;
            if ( a == 0.0f && b == 0.0f )
                unit->object.body_damage_decay_timer = -1;
        }

        int shield_decay_elapsed = unit->object.shield_damage_decay_timer;
        if ( shield_decay_elapsed != -1 )
        {
            int next = shield_decay_elapsed + 1;
            unit->object.shield_damage_decay_timer = next;
            if ( next >= 0 )
                unit->object.current_shield_damage = unit->object.current_shield_damage - 0.016666668f;
            if ( next >= 60 )
                unit->object.recent_shield_damage = unit->object.recent_shield_damage - 0.016666668f;
            float a = unit->object.current_shield_damage;
            float b = unit->object.recent_shield_damage;
            a = (a >= 0.0f) ? a : 0.0f;
            b = (b >= 0.0f) ? b : 0.0f;
            unit->object.current_shield_damage = a;
            unit->object.recent_shield_damage = b;
            if ( a == 0.0f && b == 0.0f )
                unit->object.shield_damage_decay_timer = -1;
        }
    }

    /* Dead biped simulated locally: flag a network sync when (shield_stun_ticks > 0) diverges from
     * the networked baseline. */
    if ( !unit->object.type && !unit->object.datum_role )
    {
        biped_datum *biped = (biped_datum *)unit;
        __int16 stun = biped->object.shield_stun_ticks;
        if ( (unsigned int)((-stun & ~stun) >> 31)
                != biped->biped.baseline.shield_stun_ticks_greater_than_zero )
            biped->object.flags |= (1u << _object_force_baseline_update_bit);
    }
}
