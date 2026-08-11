/* actor_start_burst @0x837B8BD0 — arm an actor's next weapon burst: latch its special-fire / new-target /
 * moving state, roll the burst duration, projectile error cone, and per-projectile damage from the firing
 * variant's ranged-combat tuning, then compute the burst's aim geometry (a return sweep around the target
 * built from the variant's burst_geometry, jittered by the global random seed and scaled by difficulty),
 * and finally raise a combat-communication event for the burst.
 *
 * Clean decompile (0x837B8BD0). Notes:
 * - the actor record's burst-state payload is resolved to named actor_datum control members,
 *   matching the sibling actor_combat_get_firing_parameters convention. Named offsets in comments below.
 * - the "moving" flag comes from the actor's current aim object velocity (salted object-datum lookup); when
 *   there is no aim object it falls back to two state bytes.
 * - the aim basis (aim_x, aim_y, aim_z) is the target-relative direction with the shipped 0.0 cross terms
 *   left verbatim so the normalize is bit-exact: (-dx, dy, 0) then unit-normalized when non-degenerate.
 * - burst_geometry fields map to the DB actor_burst_geometry members; the duration bounds are read at
 *   +0x14/+0x18 (burst_duration_lower/upper_bound). */

#include <stdint.h>
#include "headers/actor_fire_target_type.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_burst_geometry.h"
#include "headers/actor_firing_pattern.h"
#include "headers/real_point3d.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/object_datum.h"
#include "headers/real_vector3d.h"
#include "headers/math_constants.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


#include "headers/ai_information_data.h"
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern uint8_t actor_combat_allow_special_fire_situation(int actor_index, int16_t special_fire_situation);
extern void actor_combat_get_firing_parameters(int actor_index, actor_variant_definition *firing_variant_definition, actor_burst_geometry **burst_geometry_reference, actor_firing_pattern **firing_pattern_reference);
extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);
extern int actor_get_weapon(int actor_index);
extern float weapon_definition_get_damage_potential(int weapon_definition_index, float *rounds_per_second);
extern void actor_combat_find_nearby_target(real_point3d *target_point, float miss_distance);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint16_t seed_random(uint32_t *seed);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern float __fsqrts(float x);
extern double __fabs(double x);
extern double tan(double x);
extern double cos(double x);
extern double sin(double x);

void actor_start_burst(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_variant_definition *firing_variant = actor_combat_get_firing_variant_definition(actor_index);
    uint8_t use_bombardment_target = 0;

    /* latch special-fire pending flag (+1540) into the active flag (+1539) unless the situation lapsed */
    if ( actor->control.next_burst_secondary
      && !actor_combat_allow_special_fire_situation(actor_index,
              firing_variant->ranged_combat.special_fire_situation) )
        actor->control.next_burst_secondary = 0;
    char special_fire = actor->control.next_burst_secondary;
    int aim_object_index = actor->input.vehicle_index;
    actor->control.next_burst_secondary = 0;
    actor->control.fire_burst_secondary = special_fire;

    /* moving flag (+1537): true when the aim object is moving faster than 1 wu/tick */
    uint8_t moving;
    if ( aim_object_index == -1 )
    {
        moving = actor->input.in_midair || actor->control.moving;
    }
    else
    {
        object_datum *aim_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, aim_object_index)->datum;
        real_vector3d *velocity = &aim_object->object.translational_velocity;   /* +0x68 */
        moving = (velocity->n[2] * velocity->n[2])
               + (velocity->n[0] * velocity->n[0] + velocity->n[1] * velocity->n[1]) > 1.0f;
    }
    actor->control.firing_while_moving = moving;

    /* new-target flag (+1536): true while the actor has recently reacquired (last-target tick +391) */
    float new_target_pattern_time = firing_variant->ranged_combat.new_target_pattern_time;
    float team_value = game_difficulty_get_team_value(_game_difficulty_new_target_delay_scale, actor->meta.team_index);
    /* DEVIATION: disasm lwz at control+0x1B0 = current_fire_target_timer (int→float via std/lfd/fcfid);
     * prior source read ___u58 union +8 (0x1AC), one member early */
    actor->control.firing_at_new_target = (float)actor->control.current_fire_target_timer < (double)(team_value * new_target_pattern_time * 30.0f);

    actor_burst_geometry *burst_geometry = 0;
    actor_firing_pattern *firing_pattern = 0;
    actor_combat_get_firing_parameters(actor_index, firing_variant, &burst_geometry, &firing_pattern);

    /* burst duration (+278 seconds); when unset, roll it from the burst-geometry duration bounds */
    float burst_duration = actor->orders.combat.override_burst_duration;
    if ( burst_duration <= 0.0f )
    {
        burst_duration = real_seed_random_range(get_global_random_seed_address(),
                burst_geometry->burst_duration_lower_bound, burst_geometry->burst_duration_upper_bound);
        if ( firing_pattern && firing_pattern->burst_duration_modifier != 0.0f )
            burst_duration = firing_pattern->burst_duration_modifier * burst_duration;
        if ( actor->external_orders.playfighting )   /* berserk: shorter bursts */
            burst_duration = burst_duration * 0.60000002f;
    }
    int16_t team_index = actor->meta.team_index;
    actor->control.fire_state_timer = (int)(burst_duration * 30.0f);   /* burst tick count (+0x5F4) */

    /* projectile error angle (+422), difficulty- and pattern-scaled */
    float projectile_error_angle = firing_variant->ranged_combat.projectile_error_angle;
    float error_team_value = game_difficulty_get_team_value(_game_difficulty_fire_projectile_error_scale, team_index);
    float error_angle = error_team_value * projectile_error_angle;
    if ( firing_pattern && firing_pattern->error_angle_modifier != 0.0f )
        error_angle = firing_pattern->error_angle_modifier * (error_team_value * projectile_error_angle);
    if ( actor->external_orders.playfighting )
        error_angle = error_angle * 2.0f + DEG_TO_RAD;   /* berserk: double + 1 degree */
    actor->control.burst_damage_modifier = 0.0f;  /* DEVIATION: stfs 0x69C(r31); was burst_error */
    actor->control.burst_error = error_angle;     /* DEVIATION: stfs 0x698(r31); was burst_aim_vector.z */

    /* per-projectile damage (+423) */
    if ( firing_variant->ranged_combat.weapon_damage_modifier <= 0.0f )
    {
        if ( firing_variant->ranged_combat.damage_per_second > 0.0f )
        {
            int weapon = actor_get_weapon(actor_index);
            if ( weapon != -1 )
            {
                float rounds_per_second;
                int weapon_definition_index = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum->definition_index;
                float damage_potential = weapon_definition_get_damage_potential(weapon_definition_index,
                        &rounds_per_second);
                float rate_of_fire = firing_variant->ranged_combat.rate_of_fire;
                if ( rate_of_fire > 0.0f && rounds_per_second > rate_of_fire )
                    rounds_per_second = firing_variant->ranged_combat.rate_of_fire;
                if ( rounds_per_second * damage_potential > 0.0f )
                    actor->control.burst_damage_modifier = firing_variant->ranged_combat.damage_per_second
                            / (rounds_per_second * damage_potential);   /* stfs 0x69C(r31) */
            }
        }
    }
    else
    {
        actor->control.burst_damage_modifier = firing_variant->ranged_combat.weapon_damage_modifier;
    }

    /* special-fire damage / error bumps */
    if ( actor->control.fire_burst_secondary || actor->control.overcharging_weapon )
    {
        if ( firing_variant->ranged_combat.special_damage_modifier > 0.0f )
            actor->control.burst_damage_modifier = firing_variant->ranged_combat.special_damage_modifier
                    * actor->control.burst_damage_modifier;              /* lfs/stfs 0x69C(r31) */
        actor->control.burst_error = firing_variant->ranged_combat.special_projectile_error
                + actor->control.burst_error;                            /* lfs/stfs 0x698(r31) */
    }

    /* bombardment target search when armed for it and the prop target is a live "combat" prop */
    /* DEVIATION: disasm lhz control+0x1A0 / lwz control+0x1A4 = current_fire_target_type / union prop index;
     * prior source read weapon_maximum_range/current_fire_target_type (each one member early) */
    if ( firing_variant->ranged_combat.weapon_bombardment_range > 0.0f && actor->control.current_fire_target_type == actor_fire_target_prop )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->control.___u58.current_fire_target_prop_index);
        int16_t prop_state = prop->state;
        if ( prop_state < _prop_state_becoming_unacknowledged || prop_state > _prop_state_acknowledged || !prop->visibility )
            use_bombardment_target = 1;
    }

    /* target position (+395..397); optionally snapped to a nearby bombardment target */
    /* DEVIATION: disasm lwz control+0x1C0/0x1C4/0x1C8 = current_fire_target_position x/y/z;
     * prior source started one member early (aiming_at_fire_target) */
    real_point3d target;
    target.n[0] = actor->control.current_fire_target_position.x;
    target.n[1] = actor->control.current_fire_target_position.y;
    target.n[2] = actor->control.current_fire_target_position.z;
    if ( use_bombardment_target )
        actor_combat_find_nearby_target(&target, firing_variant->ranged_combat.weapon_bombardment_range);

    /* aim basis from the target-relative direction (shipped 0.0 cross terms kept verbatim) */
    float dx = target.n[0] - actor->input.position.head_position.x;
    float dy = target.n[1] - actor->input.position.head_position.y;
    float dz = target.n[2] - actor->input.position.head_position.z;
    float aim_x = (dz * 0.0f) - dx;
    float aim_y = dy - (dz * 0.0f);
    float aim_z = (dx * 0.0f) - (dy * 0.0f);
    float magnitude = __fsqrts(aim_y * aim_y + (aim_z * aim_z + aim_x * aim_x));
    if ( __fabs(magnitude) >= 0.000099999997f )
    {
        float inverse = 1.0f / magnitude;
        aim_y = inverse * aim_y;
        aim_x = inverse * aim_x;
        aim_z = inverse * aim_z;
    }

    /* randomly mirror the sweep direction */
    if ( seed_random(get_global_random_seed_address()) > 0x8000u )
    {
        aim_y = -aim_y;
        aim_x = -aim_x;
        aim_z = -aim_z;
    }

    /* burst sweep angles: origin swing then return swing, jittered and difficulty-scaled */
    float origin_angle_range = burst_geometry->burst_origin_angle;
    float origin_angle = real_seed_random_range(get_global_random_seed_address(),
            -origin_angle_range, origin_angle_range);
    float return_angle_range = burst_geometry->burst_return_angle;
    float return_angle = real_seed_random_range(get_global_random_seed_address(),
            -return_angle_range, return_angle_range) + origin_angle;

    float origin_radius = game_difficulty_get_team_value(_game_difficulty_burst_error_scale, actor->meta.team_index)
            * burst_geometry->burst_origin_radius;
    float return_length = real_seed_random_range(get_global_random_seed_address(),
            burst_geometry->burst_return_length_lower_bound, burst_geometry->burst_return_length_upper_bound);
    float return_radius = game_difficulty_get_team_value(_game_difficulty_burst_error_scale, actor->meta.team_index) * return_length;
    if ( actor->external_orders.playfighting )
    {
        origin_radius = origin_radius * 2.0f;
        return_radius = return_radius * 2.0f;
    }

    /* cap the sweep to the weapon's maximum angular velocity, trimming the burst length to match */
    int burst_ticks = actor->control.fire_state_timer;
    if ( burst_ticks > 0 )
    {
        float max_angular_velocity = burst_geometry->burst_maximum_angular_velocity;
        if ( max_angular_velocity > 0.0f )
        {
            double sweep = ((float)burst_ticks * max_angular_velocity) * SECONDS_PER_TICK;
            if ( sweep > 0.78539819f )   /* clamp to pi/4 */
                sweep = 0.78539819f;
            double sweep_tangent = tan(sweep);
            /* DEVIATION: disasm lfs control+0x1CC = current_fire_target_range (was position.z, one member early) */
            float sweep_limit = actor->control.current_fire_target_range * (float)sweep_tangent;
            if ( origin_radius > sweep_limit )
            {
                if ( origin_radius >= sweep_limit * 1.5f )
                {
                    return_radius = (sweep_limit * 1.5f / origin_radius) * return_radius;
                    origin_radius = sweep_limit * 1.5f;
                    actor->control.fire_state_timer = (int)((float)burst_ticks * 1.5f);
                }
                else
                {
                    actor->control.fire_state_timer = (int)((origin_radius / sweep_limit) * (float)burst_ticks);
                }
            }
        }
    }

    float cos_origin = (float)cos(origin_angle);
    float sin_origin = (float)sin(origin_angle);
    float cos_return = (float)cos(return_angle);
    float sin_return = (float)sin(return_angle);

    int final_ticks = actor->control.fire_state_timer;

    /* origin sweep vector (shipped 0.0 cross terms verbatim) */
    float origin_vec_z = ((aim_z * cos_origin) + sin_origin) * origin_radius;
    float origin_vec_x = ((aim_y * cos_origin) + sin_origin * 0.0f) * origin_radius;
    float origin_vec_y = ((aim_x * cos_origin) + sin_origin * 0.0f) * origin_radius;

    /* return sweep vector */
    float return_vec_z = -((aim_z * cos_return) + sin_return) * return_radius;
    float return_vec_x = -((aim_y * cos_return) + sin_return * 0.0f) * return_radius;
    float return_vec_y = -((aim_x * cos_return) + sin_return * 0.0f) * return_radius;

    if ( final_ticks > 0 )
    {
        float inverse_ticks = 1.0f / (float)final_ticks;
        return_vec_x = inverse_ticks * return_vec_x;
        return_vec_y = inverse_ticks * return_vec_y;
        return_vec_z = inverse_ticks * return_vec_z;
    }

    /* DEVIATION: disasm stw control+0x1E0..0x1E8 / +0x1F8..0x20C, stfs +0x210..0x218 — the whole prior
     * store block was shifted one member early (started at current_fire_target_distance @0x1DC). True
     * targets: burst_initial_position = target, burst_relative_position = origin sweep,
     * burst_adjustment = return sweep, burst_target = initial + relative (x/y/z). */
    actor->control.burst_initial_position.x = target.n[0];
    actor->control.burst_initial_position.y = target.n[1];
    actor->control.burst_initial_position.z = target.n[2];
    actor->control.burst_relative_position.n[0] = origin_vec_x;
    actor->control.burst_relative_position.n[1] = origin_vec_y;
    actor->control.burst_relative_position.n[2] = origin_vec_z;
    actor->control.burst_adjustment.n[0] = return_vec_x;
    actor->control.burst_adjustment.n[1] = return_vec_y;
    actor->control.burst_adjustment.n[2] = return_vec_z;
    actor->control.burst_target.x = actor->control.burst_initial_position.x + actor->control.burst_relative_position.n[0];
    actor->control.burst_target.y = actor->control.burst_initial_position.y + actor->control.burst_relative_position.n[1];
    actor->control.burst_target.z = actor->control.burst_initial_position.z + actor->control.burst_relative_position.n[2];

    /* combat-communication event, if the actor is aware enough (state word +55 >= 7) */
    if ( actor->state.combat_status >= _actor_combat_status_visible )
    {
        char prop_special = 0;
        int cause_unit_index = -1;
        /* DEVIATION: same one-member-early shift corrected (type @0x1A0, prop index @0x1A4) */
        if ( actor->control.current_fire_target_type == actor_fire_target_prop )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->control.___u58.current_fire_target_prop_index);
            prop_special = prop->ally;
            cause_unit_index = prop->unit_index;
        }

        int16_t communication_type;
        if ( actor->emotions.berserk )                 /* berserk */
            communication_type = _ai_communication_shooting_berserk;
        else if ( prop_special )
            communication_type = _ai_communication_shooting_traitor;
        else if ( actor->situation.cumulative_threats[1] < 5 )
            communication_type = actor->input.vehicle_gunner ? _ai_communication_shooting_vehicle : _ai_communication_shooting;
        else
            communication_type = _ai_communication_shooting_group;

        ai_communication_event(communication_type, actor->meta.unit_index, cause_unit_index, _comm_hostility_enemy, -1, -1, 0);
    }
}