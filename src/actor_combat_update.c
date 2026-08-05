/* actor_combat_update @ 0x837B9788 — the actor's per-frame firing controller. It ages the combat timers,
 * resolves the current target (prop, point, or none) into the target-tracking fields, decides whether the actor
 * may fire (range, line of sight, special-fire modes, weapon constraints, posture gates), runs the burst/pause
 * state machine (actor+1522), computes the aim point with target tracking + leading and a weapon-aim solve,
 * tests the line of fire, and finally drives the unit's primary/secondary triggers.
 *
 * 2026-07-13: fully migrated to typed access — character (actor_definition), unit_state
 * (actor_variant_definition), prop reads (prop_datum), and the special-fire "charge counter" check, which
 * turned out to be weapon.triggers.count (mode 1 needs >=1 trigger, mode 2 fires the second trigger so it
 * needs >=2). weapon.ai_minimum_target_range is at 0x40C.
 * Several PPC bit-twiddles (cntlzw visibility/sign tests, PAIR64 sign extraction) are reconstructed as the plain
 * comparisons they compute, with deviation comments. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_definition_flags2_flags.h"
#include "headers/actor_variant_definition_flags.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_variant_definition.h"
#include "headers/weapon_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_firing_pattern.h"
#include "headers/actor_burst_geometry.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"

#include <math.h>
#include "headers/actor_fire_state.h"
#include "headers/actor_target_type.h"
#include "headers/actor_fire_target_type.h"
#include "headers/actor_special_fire_mode.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/direction_specification_type.h"
#include "headers/game_difficulty_value.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_special_fire_situation.h"
#include "headers/ai_information_data.h"
extern float normalize2d(real_vector2d *v);


extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern weapon_definition *actor_get_weapon_definition(int actor_index);
extern int actor_get_weapon(uint16_t actor_index);
extern uint8_t actor_has_ranged_weapon(uint16_t actor_index);
extern int16_t unit_get_grenade_count(int unit_index, int16_t grenade_type);
extern int16_t unit_add_grenade_type_to_inventory(int unit_index, int16_t grenade_type, int16_t grenade_count);
extern void actor_unit_control_throw_grenade(uint16_t actor_index);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t actor_combat_enable_special_fire_situation(uint16_t actor_index, int16_t special_fire_situation);
extern uint8_t actor_combat_allow_special_fire_situation(uint16_t actor_index, int16_t special_fire_situation);
extern unsigned int *players_get_combined_pvs(void);
extern int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index, const real_point3d *p1, int16_t p1_cluster_index, int16_t mode, uint8_t test_line_of_fire, int ignore_object_index, uint8_t ignore_vehicles);
extern uint8_t weapon_aim(int weapon_index, int16_t trigger_index, const real_point3d *origin, const real_point3d *target_point, uint8_t lob, real_vector3d *result_aim_vector, float *result_ticks, float *result_distance, uint8_t *result_linear);
extern void actor_get_weapon_vector(uint16_t actor_index, real_vector3d *weapon_vector);
extern uint8_t actor_move_animation_busy(uint16_t actor_index);
extern uint8_t actor_start_first_burst_delay(uint16_t actor_index, const actor_variant_definition *firing_variant_definition);
extern void actor_start_pause(uint16_t actor_index);
extern void actor_start_burst(int actor_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern float weapon_estimate_time_to_target(int weapon_index, int16_t trigger_index, float target_distance);
extern void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position, real_vector3d *desired_facing, real_vector3d *desired_gun_offset, real_point3d *estimated_position);
extern uint8_t ai_test_line_of_fire(int actor_index, int ignore_unit_index, const real_point3d *origin, const real_vector3d *vector, int *prop_index_reference);
extern void actor_combat_get_firing_parameters(int actor_index, actor_variant_definition *firing_variant_definition, actor_burst_geometry **burst_geometry_reference, actor_firing_pattern **firing_pattern_reference);
extern void actor_unit_control_primary_trigger(uint16_t actor_index, uint8_t trigger, float analog_value);
extern void actor_unit_control_secondary_trigger(uint16_t actor_index, uint8_t trigger);

void actor_combat_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);   /* actor+88 */
    actor_variant_definition *unit_state = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);   /* actor+92 */
    actor_variant_definition *firing_variant_definition = actor_combat_get_firing_variant_definition(actor_index);
    weapon_definition *weapon_definition_ = actor_get_weapon_definition(actor_index);
    int  burst_state = -1;
    uint8_t oversteer_unaligned = 0;
    uint8_t special_eval = 0;   /* decompiler v95: set when the special-fire branch ran (final_state != 4) */
    int  weapon = actor_get_weapon(actor_index);

    /* age the combat timers */
    int16_t t;
    t = actor->control.fire_state_timer; if ( t > 0 ) actor->control.fire_state_timer = t - 1;
    t = actor->control.burst_disable_timer; if ( t > 0 ) actor->control.burst_disable_timer = t - 1;
    t = actor->control.trigger_delay_timer; if ( t > 0 ) actor->control.trigger_delay_timer = t - 1;
    t = actor->control.special_fire_delay; if ( t > 0 ) actor->control.special_fire_delay = t - 1;
    if ( actor->control.current_fire_target_type > actor_fire_target_none )
        ++actor->control.current_fire_target_timer;                            /* target-stable tick count */

    /* maintain the current target kind/identity (actor+1548 = 0 none, 1 prop, 2 point) */
    if ( actor->control.fire_state != actor_fire_state_bursting )
    {
        int  new_kind = 0;
        char target_changed = 0;
        if ( actor->orders.combat.shoot_at_target )
        {
            if ( actor->orders.combat.use_manual_target_point )
                new_kind = actor_fire_target_manual_point;
            else
                new_kind = actor->target.target_prop_index != -1;
        }

        if ( new_kind == actor->control.current_fire_target_type )
        {
            if ( new_kind == actor_fire_target_prop )
            {
                target_changed = actor->control.___u58.current_fire_target_prop_index != actor->target.target_prop_index;
            }
            else if ( new_kind == actor_fire_target_manual_point )
            {
                float dx = actor->control.___u58.current_fire_target_manual_point.x - actor->output.animation.alignment_vector.__s1.i;
                float dy = actor->control.___u58.current_fire_target_manual_point.y - actor->output.animation.alignment_vector.__s1.j;
                /* DEVIATION: DB types animation.alignment_vector as real_vector2d (8B); the decompiler's
                 * .z read lands one float past it. Preserved byte-exact via explicit +8 offset. See escalation. */
                float dz = actor->control.___u58.current_fire_target_manual_point.z - *(float *)((char *)&actor->output.animation.alignment_vector + 8);
                target_changed = (dz * dz + (dx * dx + dy * dy)) > 0.25f;
            }
        }
        else
        {
            target_changed = 1;
        }

        if ( target_changed )
            actor->control.current_fire_target_timer = 0;
        actor->control.current_fire_target_type = new_kind;
        if ( new_kind == actor_fire_target_prop )
        {
            actor->control.___u58.current_fire_target_prop_index = actor->target.target_prop_index;
        }
        else if ( new_kind == actor_fire_target_manual_point )
        {
            /* DEVIATION: decompiler word-punned this 12-byte real_point3d copy (lwz/stw triple, 0x610..0x618); plain struct assignment */
            actor->control.___u58.current_fire_target_manual_point = actor->orders.combat.target_point;
        }
    }

    actor->control.aiming_at_fire_target = 0;
    float maximum_firing_range = actor_has_ranged_weapon(actor_index)
                               ? firing_variant_definition->ranged_combat.maximum_firing_range : 0.0f;
    actor->control.weapon_maximum_range = maximum_firing_range;

    uint8_t want_fire = 0;
    if ( actor->orders.combat.throw_grenade )                     /* throw grenade this frame */
    {
        int16_t grenade_type = (uint16_t)unit_state->grenade_combat.grenade_type;
        if ( grenade_type != 0xFFFF && !unit_get_grenade_count(actor->meta.unit_index, grenade_type) )
            unit_add_grenade_type_to_inventory(actor->meta.unit_index, unit_state->grenade_combat.grenade_type, 1);
        actor_unit_control_throw_grenade(actor_index);
        ai_communication_event(_ai_communication_grenade_throwing, actor->meta.unit_index, -1, -1, -1, -1, NULL);
        want_fire = 0;
        goto resolve_burst;
    }

    if ( !actor_has_ranged_weapon(actor_index) )
    {
        want_fire = 0;
        goto resolve_burst;
    }

    if ( actor->control.fire_state == actor_fire_state_wild )                         /* already in special-fire burst */
    {
        want_fire = 1;
    }
    else
    {
        special_eval = 1;
        if ( firing_variant_definition->ranged_combat.special_fire_mode > 0 )
        {
            uint8_t special_fire = 1;
            if ( actor->control.fire_state == actor_fire_state_bursting || actor->control.special_fire_delay > 0 || actor->control.special_fire_deny_attempts > 0 )
                special_fire = 0;
            if ( special_fire )
            {
                int  weapon_object = ((int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum);
                /* recovered: raw weapon_tag+1276 = weapon.triggers.count (tag_block at _weapon_definition+0x1F4) */
                weapon_definition *weapon_tag_def = TAG_GET(weapon_definition, *(int *)weapon_object);
                game_difficulty_get_team_value(_game_difficulty_special_fire_delay_scale, actor->meta.team_index);
                int16_t special_fire_mode = (uint16_t)firing_variant_definition->ranged_combat.special_fire_mode;
                if ( special_fire_mode == _actor_special_fire_mode_overcharge )
                {
                    game_difficulty_get_team_value(_game_difficulty_overcharge_chance_scale, actor->meta.team_index);
                    /* decompiler PAIR64 sign extraction reduces to: weapon must have at least one trigger */
                    if ( weapon_tag_def->weapon.triggers.count <= 0 )
                        special_fire = 0;
                }
                else if ( special_fire_mode == _actor_special_fire_mode_secondary )
                {
                    /* (x>=0)+(x<=1) & 1 == 1  <=>  x > 1 — mode 2 fires the SECOND trigger, so one isn't enough */
                    special_fire = weapon_tag_def->weapon.triggers.count > 1;
                }
            }
            if ( special_fire )
                special_fire = actor_combat_enable_special_fire_situation(actor_index, firing_variant_definition->ranged_combat.special_fire_situation);
            if ( special_fire )
            {
                unsigned int *seed = get_global_random_seed_address();
                float delay = firing_variant_definition->ranged_combat.special_fire_delay + real_seed_random_range(seed, 0.0f, 1.5f);
                unsigned int *seed2 = get_global_random_seed_address();
                float roll = real_seed_random(seed2);
                actor->control.special_fire_delay = (int)(delay * 30.0f);
                special_fire = roll < firing_variant_definition->ranged_combat.special_fire_chance;
            }
            if ( special_fire )
                special_fire = actor_combat_allow_special_fire_situation(actor_index, firing_variant_definition->ranged_combat.special_fire_situation);
            if ( special_fire )
            {
                if ( firing_variant_definition->ranged_combat.special_fire_situation == _actor_special_fire_situation_strafing )
                    actor->control.special_fire_deny_attempts = 3;
                int16_t mode = (uint16_t)firing_variant_definition->ranged_combat.special_fire_mode;
                if ( mode == 1 )
                    actor->control.overcharging_weapon = 1;
                else if ( mode == 2 )
                    actor->control.next_burst_secondary = 1;
            }
        }

        int16_t target_kind = actor->control.current_fire_target_type;
        if ( target_kind > 0 )
        {
            if ( target_kind == 1 )
            {
                prop_datum *prop = DATUM_GET(prop_data, prop_datum, actor->control.___u58.current_fire_target_prop_index);
                actor->control.current_fire_target_range = prop->distance;
                actor->control.current_fire_target_position.x = prop->center_of_mass.x;
                actor->control.current_fire_target_position.y = prop->center_of_mass.y;
                actor->control.current_fire_target_position.z = prop->center_of_mass.z;
                actor->control.current_fire_target_line_of_sight = prop->line_of_sight;
                actor->control.current_fire_target_underwater = prop->underwater;
                const unsigned int *combined_pvs = players_get_combined_pvs();
                actor->control.current_fire_target_outside_active_area = 1;
                uint16_t cluster = prop->body_location.cluster_index;
                if ( cluster != 0xFFFF )
                    /* cntlzw test: visible iff PVS bit set; actor+1572 = "not in PVS" */
                    actor->control.current_fire_target_outside_active_area = !BIT_VECTOR_TEST_FLAG(combined_pvs, (int16_t)cluster);
            }
            else
            {
                int stable_ticks = actor->control.current_fire_target_timer;
                actor->control.current_fire_target_position.x = actor->control.___u58.current_fire_target_manual_point.x;    /* target point */
                actor->control.current_fire_target_position.y = actor->control.___u58.current_fire_target_manual_point.y;
                actor->control.current_fire_target_position.z = actor->control.___u58.current_fire_target_manual_point.z;
                float dx = actor->control.___u58.current_fire_target_manual_point.x - actor->input.position.head_position.x;
                float dy = actor->control.___u58.current_fire_target_manual_point.y - actor->input.position.head_position.y;
                float dz = actor->control.___u58.current_fire_target_manual_point.z - actor->input.position.head_position.z;
                actor->control.current_fire_target_underwater = 0;
                actor->control.current_fire_target_outside_active_area = 0;
                actor->control.current_fire_target_range = sqrtf(dz * dz + (dx * dx + dy * dy));   /* decompiler __fsqrts */
                if ( !(stable_ticks % 10) )
                    actor->control.current_fire_target_line_of_sight = ai_test_line_of_sight(
                                                    &actor->input.position.head_position, actor->input.position.body_location.cluster_index, /* recovered: *((__int16*)actor+164) -> input.position.body_location.cluster_index */
                                                    &actor->control.current_fire_target_position, -1, 0, 0, -1,
                                                    actor->input.vehicle_index != -1);
            }

            actor->control.current_fire_target_superballistic = 0;
            float weapon_super_ballistic_range = firing_variant_definition->ranged_combat.weapon_super_ballistic_range;
            if ( weapon_super_ballistic_range > 0.0f && actor->control.current_fire_target_range > weapon_super_ballistic_range )
                actor->control.current_fire_target_superballistic = 1;
            uint8_t bombardment = (actor->orders.combat.bombard_target && firing_variant_definition->ranged_combat.weapon_bombardment_range > 0.0f) ? 1 : 0;
            actor->control.current_fire_target_bombardment = bombardment;
            if ( !weapon_aim(weapon, 0, &actor->input.position.head_position, &actor->control.current_fire_target_position,
                             actor->control.current_fire_target_superballistic, &actor->control.current_fire_target_aim_vector, NULL, &actor->control.current_fire_target_distance, NULL) )
                actor->control.current_fire_target_type = actor_fire_target_none;
        }

        /* posture / state gates that suppress firing */
        if ( !actor->control.current_fire_target_type || actor->control.current_fire_target_outside_active_area
          || (!actor->orders.combat.override_firing_restrictions && actor->control.burst_disable_timer > 0) )
        {
            want_fire = 0;
            goto resolve_burst;
        }
        if ( actor_move_animation_busy(actor_index) )
        {
            want_fire = 0;
            goto resolve_burst;
        }
        uint8_t firing_while_moving = actor->orders.combat.override_firing_restrictions;
        if ( !actor->orders.combat.override_firing_restrictions
          && ((actor->input.in_midair && !actor->state.flying && (unit_state->flags & (1u << _actor_variant_definition_can_shoot_while_flying_bit)) == 0)
              || ((character->flags & (1u << _actor_definition_must_crouch_to_fire_bit)) != 0 && !actor->control.crouching)
              || ((character->flags2 & (1u << _actor_definition_flags2_must_stand_to_fire_bit)) != 0 && actor->control.crouching)
              || ((character->flags2 & (1u << _actor_definition_flags2_must_stop_to_fire_bit)) != 0 && actor->control.moving)) )
        {
            want_fire = 0;
            goto resolve_burst;
        }
        if ( actor->control.current_fire_target_underwater )       { want_fire = 0; goto resolve_burst; }
        if ( actor->input.underwater )        { want_fire = 0; goto resolve_burst; }
        if ( weapon_definition_ )
        {
            float ai_minimum_target_range = weapon_definition_->weapon.ai_minimum_target_range;
            if ( ai_minimum_target_range > 0.0f && actor->control.current_fire_target_range < ai_minimum_target_range )
            {
                want_fire = 0;
                goto resolve_burst;
            }
        }
        if ( !actor->orders.look.primary_priority || actor->orders.look.primary_direction.type != _direction_specification_target || actor->control.aiming_away_from_primary )
        {
            want_fire = 0;
            goto resolve_burst;
        }

        if ( actor->control.fire_state == actor_fire_state_bursting )
        {
            want_fire = 1;
        }
        else
        {
            uint8_t has_los = (!actor->control.current_fire_target_line_of_sight || actor->control.current_fire_target_line_of_sight == _ai_line_of_sight_occluded) ? 1 : 0;
            actor->control.aiming_at_fire_target = has_los;
            if ( (!has_los && !actor->control.current_fire_target_bombardment)
              || (!firing_while_moving && actor->control.current_fire_target_range >= actor->control.weapon_maximum_range) )
            {
                want_fire = 0;
                goto resolve_burst;
            }
            actor->control.aiming_at_fire_target = 1;
            want_fire = 1;
            if ( (character->flags & (1u << _actor_definition_start_firing_before_aligned_bit)) == 0 )
            {
                float aim_threshold = actor->control.current_fire_target_range >= 1.5f
                                    ? 0.97000003f
                                    : (actor->control.current_fire_target_range * 0.17526217f + 0.70710677f);
                real_vector3d weapon_vector;
                actor_get_weapon_vector(actor_index, &weapon_vector);
                float aim_dot = weapon_vector.n[0] * actor->control.current_fire_target_aim_vector.n[0]
                              + actor->control.current_fire_target_aim_vector.n[1] * weapon_vector.n[1]
                              + actor->control.current_fire_target_aim_vector.n[2] * weapon_vector.n[2];
                if ( aim_dot < aim_threshold )
                    oversteer_unaligned = 1;
            }
        }
    }

resolve_burst:
    if ( !want_fire )
        actor->control.fire_state = actor_fire_state_none;
    {
        uint16_t state = (uint16_t)actor->control.fire_state;
        if ( state <= actor_fire_state_wild )
        {
            if ( actor->control.fire_state )
            {
                if ( state == actor_fire_state_holding || state == actor_fire_state_pausing )
                {
                    if ( !oversteer_unaligned && !actor->control.fire_state_timer )
                        burst_state = actor_fire_state_bursting;
                }
                else if ( state == actor_fire_state_bursting )
                {
                    if ( !actor->control.fire_state_timer )
                        burst_state = actor_fire_state_pausing;
                }
                else
                {
                    burst_state = (actor->control.fire_state_timer == 0) - 1;
                }
            }
            else if ( want_fire )
            {
                burst_state = actor_fire_state_holding;
            }
        }
    }

    if ( burst_state != -1 )
    {
        if ( burst_state == actor_fire_state_holding )
        {
            if ( !actor_start_first_burst_delay(actor_index, firing_variant_definition) && !oversteer_unaligned )
            {
                burst_state = actor_fire_state_bursting;
                actor_start_burst(actor_index);
                actor->control.fire_state = burst_state;
            }
            /* else: keep delay state, fall through to publish */
            else
            {
                actor->control.fire_state = burst_state;
            }
        }
        else if ( burst_state == actor_fire_state_bursting )
        {
            actor_start_burst(actor_index);
            actor->control.fire_state = burst_state;
        }
        else
        {
            if ( burst_state == actor_fire_state_pausing )
                actor_start_pause(actor_index);
            actor->control.fire_state = burst_state;
        }
    }

    /* ---- aim solve + line-of-fire ---- */
    int16_t final_state = actor->control.fire_state;
    uint8_t firing = 0;
    actor->control.burst_aim_by_vector = 0;
    uint8_t fired_special = 0;

    if ( final_state == actor_fire_state_wild )
    {
        firing = 1;
    }
    else if ( final_state == actor_fire_state_bursting )
    {
        float *aim_point = &actor->control.burst_origin.x;
        int   ignore_unit = -1;
        /* DEVIATION: decompiler word-punned this 12-byte real_point3d copy (lwz/stw triple, 0x64C..0x654 -> 0x658..0x660); plain struct assignment */
        actor->control.burst_origin = actor->control.burst_initial_position;

        if ( (uint16_t)actor->control.current_fire_target_type == actor_fire_target_prop )
        {
            prop_datum *prop = DATUM_GET(prop_data, prop_datum, actor->control.___u58.current_fire_target_prop_index);
            ignore_unit = prop->attached_to_unit_index;
            float track = game_difficulty_get_team_value(_game_difficulty_target_tracking_bonus, actor->meta.team_index) + firing_variant_definition->ranged_combat.target_track_fraction;
            if ( (track >= 1.0f || track > 0.0f) && !actor->control.current_fire_target_bombardment )
            {
                float tf = firing_variant_definition->ranged_combat.target_track_fraction;
                *aim_point          += tf * (prop->center_of_mass.x - actor->control.burst_initial_position.x);
                actor->control.burst_origin.y += (prop->center_of_mass.y - actor->control.burst_initial_position.y) * tf;
                actor->control.burst_origin.z += (prop->center_of_mass.z - actor->control.burst_initial_position.z) * tf;
            }
            float lead = game_difficulty_get_team_value(_game_difficulty_target_leading_bonus, actor->meta.team_index) + firing_variant_definition->ranged_combat.target_lead_fraction;
            if ( lead >= 1.0f || lead > 0.0f )
            {
                float time_to_target = weapon_estimate_time_to_target(weapon, actor->control.fire_burst_secondary != 0,
                                                                      actor->control.current_fire_target_distance);
                float lf = firing_variant_definition->ranged_combat.target_lead_fraction;
                *aim_point          += (prop->velocity.n[0] * time_to_target) * lf;
                actor->control.burst_origin.y += (prop->velocity.n[1] * time_to_target) * lf;
                actor->control.burst_origin.z += (prop->velocity.n[2] * time_to_target) * lf;
            }
        }

        float *aim_smoothed = &actor->control.burst_target.x;
        float ax = actor->control.burst_relative_position.n[0] + actor->control.burst_adjustment.n[0];
        actor->control.burst_relative_position.n[0] += actor->control.burst_adjustment.n[0];
        actor->control.burst_relative_position.n[1] += actor->control.burst_adjustment.n[1];
        actor->control.burst_relative_position.n[2] += actor->control.burst_adjustment.n[2];
        actor->control.burst_target.x = ax + *aim_point;
        actor->control.burst_target.y = actor->control.burst_origin.y + actor->control.burst_relative_position.n[1];
        actor->control.burst_target.z = actor->control.burst_origin.z + actor->control.burst_relative_position.n[2];

        real_point3d origin;
        real_vector3d facing;
        if ( actor->input.vehicle_index != -1 )
        {
            unit_get_camera_position(actor->meta.unit_index, &origin);
        }
        else
        {
            const real_vector3d *gun_offset;
            float check;
            if ( actor->control.crouching )                                  /* crouched */
            {
                gun_offset = &firing_variant_definition->ranged_combat.gun_offset_crouch;
                float gx = gun_offset->n[0], gy = gun_offset->n[1], gz = gun_offset->n[2];
                check = gz * gz + (gx * gx + gy * gy);
                if ( check <= 0.000099999997f )
                    gun_offset = &character->perception.gun_offset_crouch;
            }
            else
            {
                gun_offset = &firing_variant_definition->ranged_combat.gun_offset_stand;
                float gx = gun_offset->n[0], gy = gun_offset->n[1], gz = gun_offset->n[2];
                check = gz * gz + (gx * gx + gy * gy);
                if ( check <= 0.000099999997f )
                    gun_offset = &character->perception.gun_offset_stand;
            }

            const real_vector3d *fallback = (actor->control.crouching)
                ? &character->perception.gun_offset_crouch
                : &character->perception.gun_offset_stand;
            float fx = fallback->n[0], fy = fallback->n[1], fz = fallback->n[2];
            if ( gun_offset == fallback && (fz * fz + (fx * fx + fy * fy)) <= 0.000099999997f )
            {
                /* no usable gun offset: aim from the actor head position directly */
                origin.n[0] = actor->input.position.head_position.x;
                origin.n[1] = actor->input.position.head_position.y;
                origin.n[2] = actor->input.position.head_position.z;
                goto aim_solve;
            }

            real_vector3d aim_facing;
            aim_facing.n[0] = actor->input.position.body_position.x - *aim_smoothed;
            aim_facing.n[1] = actor->input.position.body_position.y - actor->control.burst_target.y;
            aim_facing.n[2] = actor->input.position.body_position.z - actor->control.burst_target.z;
            if ( normalize2d((real_vector2d *)&aim_facing) <= 0.0f )
            {
                aim_facing.n[0] = actor->input.facing_vector.n[0];
                aim_facing.n[1] = actor->input.facing_vector.n[1];
                aim_facing.n[2] = actor->input.facing_vector.n[2];
            }
            else
            {
                aim_facing.n[2] = 0.0f;
            }
            unit_estimate_position(actor->meta.unit_index, 3, &actor->input.position.body_position, &aim_facing,
                                   (real_vector3d *)gun_offset, &origin);
        }

aim_solve:
        weapon_aim(weapon, actor->control.fire_burst_secondary != 0, &origin, &actor->control.burst_target,
                   actor->control.current_fire_target_superballistic,
                   &actor->control.burst_aim_vector, NULL, NULL, NULL);
        actor->control.burst_aim_by_vector = (special_eval == 0);   /* decompiler (cntlzw(v95) & 0x20) != 0  <=>  v95 == 0 */

        real_vector3d to_origin;
        to_origin.n[0] = *aim_smoothed - origin.n[0];
        to_origin.n[1] = actor->control.burst_target.y - origin.n[1];
        to_origin.n[2] = actor->control.burst_target.z - origin.n[2];
        int prop_pattern = -1;
        if ( ai_test_line_of_fire(actor_index, ignore_unit, &origin, &to_origin, &prop_pattern) )
        {
            actor->control.blocked_communication_timer = 0;
            if ( !actor->control.fire_burst_secondary )
                firing = 1;
            else
                fired_special = 1;
        }
        else
        {
            int16_t no_lof = (int16_t)(actor->control.blocked_communication_timer + 1);
            ++actor->control.fire_state_timer;
            actor->control.blocked_communication_timer = no_lof;
            if ( no_lof >= 45 && actor->target.target_type >= actor_target_acknowledged_enemy )
            {
                int cause_unit = -1;
                if ( prop_pattern != -1 )
                    cause_unit = DATUM_GET(prop_data, prop_datum, prop_pattern)->unit_index;
                ai_communication_event(_ai_communication_blocked, actor->meta.unit_index, cause_unit, _comm_hostility_friend, -1, -1, NULL);
                actor->control.blocked_communication_timer = 0;
            }
        }
    }

    /* ---- trigger output ---- */
    float analog = 0.0f;
    uint8_t primary = 0;
    uint8_t secondary = 0;
    if ( !firing )
    {
        if ( fired_special )
        {
            if ( !actor->control.overcharging_weapon )
                secondary = 1;
            else
                actor->control.overcharging_weapon = 0;
        }
        else if ( actor->control.overcharging_weapon )                /* queued shot: fire primary, leave flag set */
        {
            analog = 1.0f;
            primary = 1;
        }
    }
    else if ( actor->control.overcharging_weapon )
    {
        actor->control.overcharging_weapon = 0;
    }
    else if ( firing_variant_definition->ranged_combat.rate_of_fire == 0.0f )     /* rate_of_fire == 0: continuous fire */
    {
        analog = 1.0f;
        primary = 1;
    }
    else if ( !actor->control.trigger_delay_timer )                /* reload timer elapsed */
    {
        primary = 1;
        analog = 1.0f;
        float rate = game_difficulty_get_team_value(_game_difficulty_rate_of_fire_scale, actor->meta.team_index);
        actor_burst_geometry *burst_geometry;
        actor_firing_pattern *firing_pattern;
        actor_combat_get_firing_parameters(actor_index, firing_variant_definition, &burst_geometry,
                                           &firing_pattern);
        if ( firing_pattern && firing_pattern->rate_of_fire_modifier > 0.0f )
            rate = firing_pattern->rate_of_fire_modifier * rate;
        int16_t reload = (int)(30.0f / rate);
        if ( reload < 2 )
            reload = 2;
        actor->control.trigger_delay_timer = reload;
    }

    actor_unit_control_primary_trigger(actor_index, primary, analog);
    actor_unit_control_secondary_trigger(actor_index, secondary);
}
