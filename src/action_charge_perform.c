/* =========================================================================
   NOTE / CAVEAT: Hex-Rays flagged this "local variable allocation has failed,
   the output may be wrong". It is a dense AI charge/melee action solver; this is
   a faithful best-effort transcription of the decompiler output. The actor's
   charge sub-state lives at actor+156 and is accessed through the DB-verified
   charge_state_data type; other actor/prop/tag fields are kept as raw byte
   offsets (the actor-AI house style, as in the action_* siblings). The original
   goto lattice (want_melee / post_melee / tail) is preserved, and the floating
   point vector math is reproduced as the decompiler expanded it; one register-
   aliased spill (the __SPAIR64__ timeout pun) is transcribed literally and should
   be re-checked against disasm (0x837F98C8) before being trusted.
   ========================================================================= */
#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/charge_state_data.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_variant_definition.h"
#include "headers/prop_datum.h"
#include "headers/actor_target_type.h"
#include "headers/charge_goal.h"
#include "headers/actor_combat_status.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"


#include "headers/weapon_definition.h"
#include "headers/ai_information_data.h"
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern weapon_definition *actor_get_weapon_definition(int actor_index);
extern uint8_t actor_has_ranged_weapon(uint16_t actor_index);
extern int game_time_get(void);
extern float normalize3d(real_vector3d *v);
extern float normalize2d(real_vector2d *v);
extern uint8_t projectile_aim_ballistic(float base_velocity, float gravity_scale, const real_point3d *origin, const real_point3d *target_point, float *target_velocity_min, float *target_ballistic_fraction_min, float *forced_velocity, uint8_t lob, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance, float *result_vertical_velocity, float *result_horizontal_velocity);
extern uint8_t unit_is_busy(int object_index);
extern uint8_t unit_melee_attack_begin(int unit_index, uint8_t continuous, const real_vector2d *alignment_vector);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern float action_charge_find_target_range(uint16_t actor_index, int16_t goal, charge_state_data *state_data);
extern uint8_t actor_move_to_prop(int actor_index, unsigned int prop_index, float accept_radius);
extern void actor_move_keep_moving_past_destination(uint16_t actor_index);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern uint8_t actor_path_has_path(uint16_t actor_index);
extern void actor_perception_unreachable(int actor_index, int prop_index, uint8_t unreachable);

extern float __fsqrts(float x);

uint8_t action_charge_perform(int actor_index) /* was: int — DB prototype */
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    charge_state_data *charge = &actor->state.action_data.___u0.charge;
    actor_definition *charge_def = TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *def2 = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    const actor_variant_definition *firing_def =
        (const actor_variant_definition *)actor_combat_get_firing_variant_definition(actor_index);
    prop_datum *prop = 0;
    real_vector3d target_dir;
    real_point3d aim[6];
    float ballistic_scratch[3]; /* [0] seeded min-velocity in, [1]=horizontal, [2]=vertical results (frame slots var_E0/var_DC/var_D8) 2026-07-31 (C4047) */
    float *target_object = 0;
    int charge_state;
    float distance;
    char do_melee;

    if (actor->target.target_prop_index == -1) /* no target prop */
    {
        charge->advancing = 0;
        goto after_charge_flags;
    }

    prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);

    if (actor->input.delayed_attached_projectile_index != -1)
        goto force_want_to_shoot;

    {
        int state = charge->goal;
        if (state == charge_goal_vehicle_ramming || state == charge_goal_vehicle_strafing)
            goto force_want_to_shoot;

        if (state != 2 && state != 3)
        {
            /* --- decide whether to enter the charge --- */
            char want_charge = 1;
            if ((charge_def->flags & (1u << _actor_definition_stalking_behavior_bit)) == 0 || actor->state.combat_status < _actor_combat_status_clear_los || actor->emotions.berserk)
                want_charge = 0;
            charge->goal = (want_charge != 0);

            if (want_charge)
            {
                char keep_charging = 1;
                if ((prop->line_of_sight && prop->line_of_sight != _ai_line_of_sight_occluded) || prop->quantized_facing > 2)
                    keep_charging = 0;
                charge->stalking_currently_exposed = keep_charging;
                charge->advancing = !keep_charging || (charge_def->flags & (1u << _actor_definition_stalking_freeze_when_exposed_bit)) == 0;
                if (keep_charging)
                    ++charge->stalking_discovery_timer;
                charge->stalking_catch_target = 0;
                if (keep_charging || prop->quantized_closing_speed > 1)
                {
                    float threshold = charge_def->defensive.stalking_max_distance;
                    if (threshold > 0.0 && prop->distance >= threshold)
                        charge->stalking_catch_target = 1;
                }
                else
                {
                    charge->stalking_catch_target = 1;
                }
                goto after_charge_flags;
            }

            if (actor_has_ranged_weapon(actor_index) && !actor->input.underwater)
            {
                float enter_range, exit_range;
                if (actor->emotions.berserk) { enter_range = firing_def->ranged_combat.berserk_firing_range_lower_bound; exit_range = firing_def->ranged_combat.berserk_firing_range_upper_bound; }
                else                         { enter_range = firing_def->ranged_combat.combat_range_lower_bound;         exit_range = firing_def->ranged_combat.combat_range_upper_bound; }

                {
                    float *weapon_def = (float *)actor_get_weapon_definition(actor_index);
                    if (weapon_def)
                    {
                        float wr = ((real_point3d *)weapon_def)[86].n[1];
                        if (wr > 0.0 && enter_range <= wr)
                            enter_range = wr;
                    }
                }

                {
                    float range_to_prop = prop->distance;
                    if (charge->advancing)
                    {
                        if (range_to_prop < enter_range)
                            charge->advancing = 0;
                    }
                    else if (range_to_prop > exit_range)
                    {
                        charge->advancing = 1;
                    }
                    if (range_to_prop > 0.69999999 && prop->line_of_sight && prop->line_of_sight != _ai_line_of_sight_occluded)
                        charge->advancing = 1;
                }
                goto after_charge_flags;
            }

force_want_to_shoot:
            charge->advancing = 1;
            goto after_charge_flags;
        }

        /* --- state 2/3: sustain / advance the charge --- */
        {
            char at_ranged = actor->emotions.berserk;
            char in_range = 1;
            float shoot_range = 3.4028235e38f; /* FLT_MAX */

            if (!actor_has_ranged_weapon(actor_index))
                at_ranged = 1;

            if (charge->launched_melee_attack || charge->launched_leap || charge->leap_pending)
            {
                in_range = 0;
            }
            else if (actor->emotions.berserk || !actor_has_ranged_weapon(actor_index))
            {
                shoot_range = at_ranged ? def2->ranged_combat.berserk_melee_abort_range : def2->ranged_combat.melee_abort_range;
            }

            if (actor->external_orders.disable_charging)
            {
                float clamp = charge_def->berserk.melee_attack_range;
                /* fsel: clamp = max(clamp, 0) */
                if (clamp < 0.0f) clamp = 0.0f;
                if (shoot_range > clamp + 0.80000001f)
                    shoot_range = clamp + 0.80000001f;
            }

            if (in_range && prop->distance > shoot_range)
            {
                charge->aborted_melee_attack = 1;
            }
            else
            {
                actor->emotions.last_melee_attack_time = game_time_get();
                charge->advancing = 1;
                if (in_range)
                {
                    if (charge->goal == charge_goal_melee)
                    {
                        if (charge_def->berserk.melee_leap_range_upper_bound == 0.0 || charge_def->berserk.melee_leap_chance == 0.0)
                            charge->leap_possible_if_at_range = 0;
                        else if (prop->flying || prop->unreachable_ticks > 0)
                            charge->leap_possible_if_at_range = 1;

                        if (charge->leap_possible_if_at_range
                            && (prop->unreachable_ticks > 0
                                || prop->distance > (charge_def->berserk.melee_leap_range_lower_bound * 1.5f)))
                            charge->goal = charge_goal_melee_leaping;
                    }
                    else if (prop->distance < charge_def->berserk.melee_leap_range_lower_bound)
                    {
                        charge->leap_possible_if_at_range = 1;
                        charge->goal = charge_goal_melee;
                    }
                }
            }
        }
    }

after_charge_flags:
    if (charge->launched_melee_attack)
    {
        int unit = actor->meta.unit_index;
        char busy_ok = 0;
        if (unit == -1 || !unit_is_busy(unit))
            busy_ok = 1;
        charge->finished_melee_attack = busy_ok;
        goto tail;
    }
    if (charge->leap_pending)
        goto tail;

    charge_state = charge->goal;
    if (charge_state != charge_goal_melee && charge_state != charge_goal_melee_leaping)
        goto tail;
    if (!prop)
        goto tail;

    do_melee = 0;

    /* --- build the direction to the target prop --- */
    if (prop->distance < 0.80000001)
    {
        target_dir.n[0] = prop->actor_to_prop.n[0];
        target_dir.n[1] = prop->actor_to_prop.n[1];
        target_dir.n[2] = prop->actor_to_prop.n[2];
        goto want_melee;
    }
    else
    {
        float lead = 0.0f;
        float speed = __fsqrts((prop->velocity.n[2] * prop->velocity.n[2])
                             + ((prop->velocity.n[0] * prop->velocity.n[0])
                                     + (prop->velocity.n[1] * prop->velocity.n[1])));
        float dt;
        target_object = ((float *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum);

        if (speed > 0.0)
            lead = (((prop->actor_to_prop.n[0] * prop->velocity.n[0]
                                 + (prop->actor_to_prop.n[1] * prop->velocity.n[1]
                                         + prop->actor_to_prop.n[2] * prop->velocity.n[2])) / speed) + 1.0f) * 0.5f;

        /* DEVIATION: decompiler rendered the std+lfd+fcfid int->float idiom (0x837F9E50) as a
         * 64-bit pun store into a scratch slot; it is just the conversion below. */
        dt = (float)charge->melee_ticks_until_dangerous;

        /* lead the aim point by the prop velocity * dt * lead */
        target_dir.n[0] = (prop->velocity.n[0] * (dt * lead) + prop->body_position.n[0]) - actor->input.position.body_position.x;
        target_dir.n[1] = (prop->velocity.n[1] * (dt * lead) + prop->body_position.n[1]) - actor->input.position.body_position.y;
        target_dir.n[2] = (prop->velocity.n[2] * (dt * lead) + prop->body_position.n[2]) - actor->input.position.body_position.z;

        /* if the aim points behind the prop facing, or normalize fails, fall
           back to the prop forward vector */
        if ((prop->actor_to_prop.n[0] * target_dir.n[0]
                  + (prop->actor_to_prop.n[2] * target_dir.n[2] + prop->actor_to_prop.n[1] * target_dir.n[1])) < 0.0
            || normalize3d(&target_dir) == 0.0)
        {
            target_dir.n[0] = prop->actor_to_prop.n[0];
            target_dir.n[1] = prop->actor_to_prop.n[1];
            target_dir.n[2] = prop->actor_to_prop.n[2];
        }
        distance = prop->distance; /* range along the aim */
    }

    if (charge_state == charge_goal_melee_leaping && !charge->launched_leap)
    {
        if (distance >= charge_def->berserk.melee_leap_range_lower_bound || prop->unreachable_ticks || prop->flying)
        {
            if (distance < charge_def->berserk.melee_leap_range_upper_bound)
            {
                float gravity = charge_def->berserk.melee_leap_velocity;
                ballistic_scratch[0] = charge_def->berserk.melee_leap_velocity * 0.30000001f;
                /* call rewritten from disasm 0x837F9F40-0x837F9F90: decompiler scrambled the 14-arg mapping (r5=actor+0x12C origin, r6=prop+0xBC target, r7=&scratch[0], r8=charge_def+0x394 melee_leap_ballistic, r9=0, r10=0 lob, stack outs aim/0/0/0/&scratch[2]/&scratch[1]) 2026-07-31 (C4047) */
                if (projectile_aim_ballistic(gravity, 1.0f, &actor->input.position.body_position, &prop->body_position,
                                             &ballistic_scratch[0], &charge_def->berserk.melee_leap_ballistic,
                                             0, 0, (real_vector3d *)aim, 0, 0, 0,
                                             &ballistic_scratch[2], &ballistic_scratch[1]))
                {
                    if (normalize2d((real_vector2d *)aim) == 0.0)
                    {
                        aim[0].n[0] = actor->input.facing_vector.n[0];
                        aim[0].n[1] = actor->input.facing_vector.n[1];
                        aim[0].n[2] = actor->input.facing_vector.n[2];
                        if (normalize2d((real_vector2d *)aim) == 0.0)
                        {
                            aim[0].n[0] = global_forward3d->n[0];
                            aim[0].n[1] = global_forward3d->n[1];
                            aim[0].n[2] = global_forward3d->n[2];
                        }
                    }
                    charge->leap_pending = 1;
                    charge->leap_horizontal_velocity = ballistic_scratch[1];
                    charge->leap_vertical_velocity = ballistic_scratch[2]; /* disasm 0x837FA000 lfs var_D8 = result_vertical_velocity out-slot, not the packed_ticks pun 2026-07-31 (C4047) */
                    /* DEVIATION: 64-bit std copy (0x837FA010) of aim.xy into the real_vector2d; plain component copy */
                    charge->leap_alignment_vector.i = aim[0].n[0];
                    charge->leap_alignment_vector.j = aim[0].n[1];
                }
            }
        }
        else
        {
            charge->aborted_melee_attack = 1;
            actor->emotions.last_melee_attack_time = -1;
        }
        goto post_melee;
    }

    if (!charge->melee_suicide)
    {
        if (charge_state == charge_goal_melee_leaping && charge->launched_leap)
            distance = -(((target_object[26] * target_dir.n[0]
                                       + (target_object[28] * target_dir.n[2] + target_object[27] * target_dir.n[1]))
                                 * (float)charge->melee_ticks_until_dangerous) - distance);
        if (distance >= (charge_def->berserk.melee_attack_range + charge->melee_danger_range))
            goto post_melee;
        goto want_melee;
    }

    if (distance < charge_def->berserk.melee_attack_range
        || (distance < charge_def->berserk.suicide_sensing_distance
            && (((prop->velocity.n[1] - target_object[27]) * target_dir.n[1])
                     + (((prop->velocity.n[0] - target_object[26]) * target_dir.n[0])
                             + ((prop->velocity.n[2] - target_object[28]) * target_dir.n[2]))) > 0.023333333))
    {
want_melee:
        do_melee = 1;
    }

post_melee:
    if (charge->leap_pending || (do_melee && !charge->melee_suicide))
    {
        real_vector2d flat;
        flat.n[0] = target_dir.n[0];
        flat.n[1] = target_dir.n[1];
        if (normalize2d(&flat) > 0.0)
        {
            float cone = charge->launched_leap ? 0.0f : 0.86602539f;
            if ((actor->input.facing_vector.n[1] * flat.n[1] + flat.n[0] * actor->input.facing_vector.n[0]) < cone)
            {
                do_melee = 0;
                charge->leap_pending = 0;
                charge->alignment_incorrect = 1;
            }
        }
    }

    if (do_melee)
    {
        real_vector2d dir;
        dir.n[0] = target_dir.n[0];
        dir.n[1] = target_dir.n[1];
        if (normalize2d(&dir) == 0.0)
        {
            dir.n[0] = actor->input.facing_vector.n[0];
            dir.n[1] = actor->input.facing_vector.n[1];
        }
        if (unit_melee_attack_begin(actor->meta.unit_index, 0, &dir))
        {
            ai_communication_event(_ai_communication_melee, actor->meta.unit_index, prop->unit_index, _comm_hostility_enemy, -1, -1, 0);
            charge->launched_melee_attack = 1;
        }
    }

tail:
    {
        int now = game_time_get();
        int state = charge->goal;

        if ((state == charge_goal_melee || state == charge_goal_melee_leaping) && !charge->launched_melee_attack && !charge->leap_pending)
        {
            if (charge->launched_leap)
            {
                if (charge->leap_failure_timer > 15)
                    charge->aborted_melee_attack = 1;
            }
            else
            {
                float timeout = charge_def->berserk.melee_attack_charge_timer;
                if (timeout > 0.0)
                {
                    int start = charge->charge_start_time;
                    /* DEVIATION: reg-alloc left the elapsed compare as a spilled
                       __SPAIR64__ pun; it tests now >= start + timeout*30. */
                    if ((float)now >= ((timeout * 30.0f) + (float)start))
                        charge->aborted_melee_attack = 1;
                }
            }
        }

        if (state == charge_goal_vehicle_strafing || state == charge_goal_vehicle_ramming)
            actor->emotions.last_vehicle_charge_time = now;

        {
            float target_range = action_charge_find_target_range(actor_index, charge->goal, charge);
            charge->acceptable_target_range = target_range;

            if (!actor->meta.swarm && actor->meta.timeslice)
            {
                char moved = 0;
                charge->unable_to_advance = 0;
                if (!charge->launched_melee_attack && !charge->launched_leap && !charge->leap_pending && charge->advancing)
                {
                    float move_range = (charge->goal == charge_goal_melee_leaping) ? 4.0f : 1.5f;
                    if (move_range <= target_range)
                        move_range = target_range;
                    if (actor_move_to_prop(actor_index, actor->target.target_prop_index, move_range))
                    {
                        actor_move_keep_moving_past_destination(actor_index);
                        moved = 1;
                    }
                    if (!moved)
                    {
                        charge->unable_to_advance = 1;
                        charge->advancing = 0;
                    }
                }
                if (!moved)
                    actor_move_halt(actor_index);

                if (actor->target.target_type >= actor_target_acknowledged_enemy)
                {
                    unsigned char unreachable = 0;
                    int st = charge->goal;
                    /* recovered: (float *)prop_data->data + 78*idx + 71 -> prop_datum.distance
                     * (78*4 = 312 = sizeof(prop_datum), +71*4 = +284); same element `prop` already holds. */
                    char range_exceeded = prop->distance > charge->acceptable_target_range;
                    if ((st == charge_goal_melee || st == charge_goal_melee_leaping) && (charge->launched_leap || charge->leap_pending || charge->launched_melee_attack))
                        range_exceeded = 0;
                    if (range_exceeded
                        && (charge->unable_to_advance || !actor_path_has_path(actor_index) || actor->control.path.path.endpoint.target_radius > charge->acceptable_target_range))
                        unreachable = 1;
                    actor_perception_unreachable(actor_index, actor->target.target_prop_index, unreachable);
                }
            }
        }

        {
            int st = charge->goal;
            if (st == charge_goal_melee || st == charge_goal_melee_leaping)
            {
                if (charge->aborted_melee_attack) return 1;
                if (charge->finished_melee_attack) return 1;
                if (charge->unable_to_advance) return 1;
                return 0;
            }
            else if (st == charge_goal_vehicle_strafing || st == charge_goal_vehicle_ramming)
            {
                return charge->unable_to_advance;
            }
            return 0;
        }
    }
}