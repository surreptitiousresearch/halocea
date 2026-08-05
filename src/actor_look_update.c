/* actor_look_update @0x837FBA10 — resolves an actor's desired facing / aiming / looking vectors
 * for the frame and publishes them to the unit-control output slots.
 *
 * The function layers candidate look directions in priority order:
 *   1. firing-burst aim (forces the aim vector while a burst is in progress)
 *   2. an explicit look request (actor orders.look primary priority/direction)
 *   3. a stimulus-driven look (actor control secondary_look state)
 *   4. idle looking (timed major/minor wandering of facing/aim/look)
 *   5. a stationary-facing lock and the final aim->look fallback copy
 * Each layer validates its candidate with actor_look_valid_aim_vector / _valid_look_vector
 * (cone tests around the current desired facing) before committing it.
 *
 * DEVIATION (signature): the decompiler promoted the cos-argument scratch FP slot to a phantom
 * `long double` 2nd parameter. Disasm + DB prototype confirm a single r3 param.
 * DEVIATION (float-slot-skip args): every actor_look_valid_aim_vector / _valid_look_vector call
 * was rendered by the decompiler with the pointer args mapped from r3 upward, so each call carried
 * a stale-r3 phantom first pointer (`valid`/`unit_busy`/decode results) and DROPPED the last real
 * argument. The leading float skips the r3 slot on this ABI: the real args are r4/r5 (aim) and
 * r4/r5/r6/r7 (look). Restored per call site from the disassembly (0x837FBB9C..0x837FC754):
 * aim = (yaw_deviation, &desired_facing [or &input.facing], &candidate); look = (yaw_deviation,
 * &look_cone_cosines, &desired_facing [or &input.facing], &desired_aiming, &candidate).
 * DEVIATION (fused slots): the decompiler fused the layer-3/facing-lock commits into 64-bit
 * spec_tail/HIDWORD word-punning. Each fusion is a whole-real_vector3d copy of the decoded
 * candidate (disasm: three lwz/stw word moves per vector); rewritten as struct assignments, with
 * the shared n[1]-store labels (LABEL_100/101/102) flattened into per-path full copies.
 * PPC `_cntlzw` bit tests are reduced to the equality comparisons they implement (marked inline).
 */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_idle_looking.h"
#include "headers/direction_specification.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_mode.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"
extern double cos(double x);
extern double __fabs(double x);

extern uint8_t actor_combat_currently_firing_burst(uint16_t actor_index);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_decode_direction(int actor_index, direction_specification *specification, real_vector3d *direction);
extern int16_t actor_action_class(uint16_t actor_index);
extern uint8_t actor_look_valid_aim_vector(float yaw_deviation, const real_vector3d *facing_vector, const real_vector3d *attempted_aiming_vector);
extern uint8_t actor_look_valid_look_vector(float yaw_deviation, const real_vector2d *cone_limits, const real_vector2d *aiming_vector, const real_vector2d *attempted_looking_vector, const real_vector2d *look_vector);
extern uint8_t actor_move_force_stop(uint16_t actor_index);
extern int actor_look_idle_timer(int actor_index, actor_idle_looking *looking_definition, int16_t timer_type, uint8_t interesting_direction);
extern uint8_t actor_look_idle_new_major_direction(int actor_index, actor_idle_looking *looking_definition, const real_vector3d *base_vector, uint8_t free_facing, uint8_t major_is_aiming, uint8_t has_minor_variation);
extern void actor_look_idle_new_minor_direction(int actor_index, actor_idle_looking *looking_definition, const real_vector3d *base_vector);
extern uint8_t unit_is_busy(int object_index);
extern void actor_stimulus_abandon_stationary_facing(uint16_t actor_index);
extern void actor_unit_control_exact_facing(uint16_t actor_index, uint8_t exact_facing);
extern float normalize2d(real_vector2d *v);
extern int actor_get_weapon(uint16_t actor_index);
extern actor_idle_looking *actor_look_get_looking_definition(uint16_t actor_index);

void actor_look_update(int actor_index)
{
    actor_datum *actor;
    uint8_t look_enabled;
    int16_t unit_action;
    /* DB: the tag at meta.definition_index is an actor_definition; every offset resolves to a
     * named actor_definition member via res.py. Retyped from the decompiler's float*. */
    const actor_definition *actor_def;
    bool can_aim;
    bool can_aim_saved;
    int actor_index_copy;
    float aim_yaw_deviation;   /* runtime aiming-deviation cosine, passed single-precision (f31) */
    float look_yaw_deviation;  /* runtime looking-deviation cosine, passed single-precision (f30) */
    uint8_t aim_pending;
    uint8_t look_pending;
    uint8_t facing_available;
    uint8_t aim_applied;
    uint8_t look_applied;
    float yaw_delta;
    float pitch_delta;
    uint8_t burst_direction_valid;
    direction_specification *empty_dir;
    int16_t look_class;
    int16_t look_mode;
    int16_t stimulus_class;
    int look_timer;
    int16_t next_timer;
    int look_class_snapshot;
    uint8_t aim_succeeded;
    uint8_t prior_face_exactly;
    uint8_t stimulus_aim_valid;
    uint8_t facing_succeeded;
    bool set_exact_facing;
    uint8_t exact_facing_value;
    actor_idle_looking *idle;
    bool has_idle_aim;
    bool has_idle_look;
    uint8_t idle_updated;
    uint8_t idle_major_new;
    uint8_t free_facing;
    int facing_timer;
    uint8_t major_is_aiming;
    uint8_t major_candidate_ok;
    uint8_t minor_has_variation;
    const real_vector3d *base_vector;
    int look_class4;
    uint8_t minor_active;
    uint8_t look_actively_engaged;
    float stationary_angle;
    float stationary_cos;
    uint8_t stationary_ok;
    /* 2d scratch copies for the stationary-facing cone test (disasm ld/std pairs — the
     * decompiler fused them into one __int128; each is a copy of a 3d vector's n[0]/n[1]) */
    real_vector2d stationary_facing2d;
    real_vector2d stationary_aiming2d;
    real_vector2d stationary_fixed2d;
    real_vector2d look_cone_cosines;       /* {cos(yaw_delta), cos(pitch_delta)} cone limits */
    direction_specification spec;          /* burst/stimulus/idle-major decode scratch */
    real_vector3d decoded;                 /* primary/burst/idle-minor decoded direction */
    direction_specification empty_storage;
    /* the binary reuses spec's stack slot as the decode TARGET for the stimulus and idle-major
     * layers — a real_vector3d overlaid on the direction_specification storage */
    real_vector3d *const spec_vector = (real_vector3d *)&spec;

    actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    look_enabled = 1;
    look_actively_engaged = 0;
    unit_action = actor->output.movement_type;
    actor_def = TAG_GET(const actor_definition, actor->meta.definition_index);

    if (unit_action == 1)
    {
        can_aim = 0;
        look_enabled = 0;
    }
    else if (actor->input.vehicle_gunner)
    {
        can_aim = 1;
    }
    else
    {
        can_aim = (!unit_action || unit_action == 2) && actor_get_weapon(actor_index) != -1;
    }
    can_aim_saved = can_aim;

    if (!look_enabled)
    {
        actor_index_copy = actor_index;
        actor->control.desired_aiming_vector = actor->control.desired_facing_vector;
        actor->control.desired_looking_vector = actor->control.desired_facing_vector;
        goto LABEL_192;
    }

    aim_yaw_deviation = actor_def->looking.runtime_maximum_aiming_deviation_cosine.__s1.yaw;
    look_yaw_deviation = actor_def->looking.runtime_maximum_looking_deviation_cosine.__s1.yaw;
    aim_pending = actor->control.free_facing_vector;
    look_pending = actor->control.optional_facing_vector;
    facing_available = 1;
    aim_applied = 0;
    look_applied = 0;

    {
        /* cone-limit cosines from the variant's yaw/pitch look deltas (firing pose vs. normal pose) */
        if (actor->state.mode == _actor_mode_combat)
        {
            yaw_delta = actor_def->looking.combat_looking_delta_angles[0];
            pitch_delta = actor_def->looking.combat_looking_delta_angles[1];
        }
        else
        {
            yaw_delta = actor_def->looking.noncombat_looking_delta_angles[0];
            pitch_delta = actor_def->looking.noncombat_looking_delta_angles[1];
        }
        look_cone_cosines.n[0] = (float)cos(yaw_delta);
        look_cone_cosines.n[1] = (float)cos(pitch_delta);

        /* layer 1: firing-burst aim. (The binary's fall-through path reloads look_class from an
         * uninitialized halfword stack slot; the value is dead — always overwritten below —
         * spelled here as _primary_priority_none.) */
        burst_direction_valid = 0;
        look_class = _primary_priority_none;
        if (actor_combat_currently_firing_burst(actor_index) && !actor->orders.combat.abort_burst)
        {
            empty_dir = direction_get_empty(&empty_storage);
            spec = *empty_dir;
            spec.type = _direction_specification_target;
            if (actor_look_decode_direction(actor_index, &spec, &decoded))
            {
                look_class = _primary_priority_locked_aiming;
                look_actively_engaged = 1;
                burst_direction_valid = 1;
            }
        }

        /* layer 2: explicit look state (actor orders.look) */
        if (!burst_direction_valid)
        {
            look_mode = actor->orders.look.primary_priority;
            look_class = look_mode;
            if (look_mode)
            {
                if (look_mode != _primary_priority_face_360)
                {
                    if (actor_look_decode_direction(actor_index, &actor->orders.look.primary_direction, &decoded))
                        look_actively_engaged = (actor->orders.look.primary_direction.type == _direction_specification_target); /* (_cntlzw(x-2)&0x20)!=0  ==  (x==2) */
                    else
                        look_class = _primary_priority_none;
                }
            }
        }

        /* layer 3: stimulus look (actor control secondary_look state) */
        stimulus_class = _secondary_look_priority_none;
        if ((uint16_t)actor->control.secondary_look_type < 0x8000u && actor->control.secondary_look_timer > 0)
        {
            if (actor_look_decode_direction(actor_index, &actor->control.secondary_look_direction, spec_vector))
                stimulus_class = actor->control.secondary_look_priority;
        }
        if (actor->control.moving)
        {
            if (actor_action_class(actor_index) == 2)
            {
                if (stimulus_class > _secondary_look_priority_turn_and_aim)
                    stimulus_class = _secondary_look_priority_turn_and_aim;
            }
        }

        /* age the stimulus look timer */
        look_timer = actor->control.secondary_look_timer;
        if (look_timer > 0)
        {
            next_timer = look_timer - 1;
            actor->control.secondary_look_timer = next_timer;
            if (!next_timer)
            {
                actor->control.secondary_look_type = _secondary_look_none;
                actor->control.secondary_look_priority = _secondary_look_priority_none;
            }
        }

        look_class_snapshot = look_class;
        actor->control.aiming_away_from_primary = 0;

        if (look_class >= _primary_priority_opportunity_aiming)
        {
            aim_succeeded = 0;
            if (look_class < _primary_priority_aiming
                || !aim_pending
                && !actor_look_valid_aim_vector(aim_yaw_deviation, &actor->control.desired_facing_vector, &decoded))
            {
                if (look_class_snapshot < _primary_priority_facing || !look_pending)
                    goto LABEL_45;
                look_pending = 0;
                aim_pending = 1;
            }
            aim_succeeded = 1;
LABEL_45:
            if (aim_succeeded)
            {
                facing_available = 0;
                actor->control.desired_aiming_vector = decoded;
                if (look_class_snapshot >= _primary_priority_locked_aiming)
                {
                    look_applied = 1;
                    if (can_aim)
                        actor->control.desired_looking_vector = decoded;
                }
            }
            if (look_class_snapshot == _primary_priority_opportunity_aiming)
                look_class = aim_pending == 0 ? _primary_priority_none : _primary_priority_aiming;
            if (aim_pending)
            {
                prior_face_exactly = actor->control.face_exactly;
                aim_pending = 0;
                actor->control.desired_facing_vector = decoded;
                look_pending = 0;
                actor->control.face_exactly = (look_class == _primary_priority_exact_facing) | prior_face_exactly; /* (_cntlzw(x-4)&0x20)!=0 == (x==4) */
            }
            aim_applied = (!actor->control.free_facing_vector && !actor->control.optional_facing_vector)
                       || look_class >= _primary_priority_locked_facing;
        }

        /* commit stimulus-class look (layer 3 resolution) */
        /* range idiom: secondary priorities idle_look(2)..stop_and_aim(6); override(7)/override_exact_facing(8) fall through to the facing-lock path below */
        if ((unsigned int)(stimulus_class - _secondary_look_priority_idle_look) > 6)
            goto LABEL_103;
        if (stimulus_class == _secondary_look_priority_idle_aim || stimulus_class == _secondary_look_priority_aim
            || stimulus_class == _secondary_look_priority_turn_and_aim || stimulus_class == _secondary_look_priority_stop_and_aim
            || stimulus_class == _secondary_look_priority_idle_look)
        {
            stimulus_aim_valid = actor_look_valid_aim_vector(aim_yaw_deviation, &actor->control.desired_facing_vector, spec_vector);
            if (!aim_applied && !look_applied)
            {
                if (stimulus_class >= _secondary_look_priority_stop_and_aim && actor_move_force_stop(actor_index)
                    || stimulus_class >= _secondary_look_priority_turn_and_aim && (look_pending || actor->control.free_facing_vector)
                    || stimulus_class >= _secondary_look_priority_aim && !stimulus_aim_valid && aim_pending && facing_available)
                {
                    if (!actor->control.face_exactly || !stimulus_aim_valid)
                    {
                        actor->control.desired_facing_vector = *spec_vector;
                        actor->control.face_exactly = 0;
                    }
LABEL_99:
                    aim_pending = 0;
                    actor->control.desired_aiming_vector = *spec_vector;
                    actor->control.desired_looking_vector = *spec_vector;
                    look_applied = 0;
                    actor->control.aiming_away_from_primary = 1;
                    facing_available = 0;
                    goto LABEL_103;
                }
            }
            if (!look_applied && stimulus_aim_valid && (stimulus_class >= _secondary_look_priority_turn_and_aim || stimulus_class >= _secondary_look_priority_idle_aim && facing_available))
            {
                actor->control.desired_looking_vector = *spec_vector;
                actor->control.desired_aiming_vector = *spec_vector;
                look_applied = 0;
                actor->control.aiming_away_from_primary = 1;
                facing_available = 0;
                goto LABEL_103;
            }
            if (can_aim)
            {
                if (actor_look_valid_look_vector(look_yaw_deviation, &look_cone_cosines,
                                                 (const real_vector2d *)&actor->control.desired_facing_vector,
                                                 (const real_vector2d *)&actor->control.desired_aiming_vector,
                                                 (const real_vector2d *)spec_vector))
                {
                    can_aim = 0;
                    actor->control.desired_looking_vector = *spec_vector;
                    goto LABEL_103;
                }
            }
            if (facing_available && stimulus_aim_valid)
            {
                can_aim = 1;
                actor->control.desired_looking_vector = *spec_vector;
                actor->control.desired_aiming_vector = *spec_vector;
                facing_available = 0;
            }
            goto LABEL_103;
        }

        /* _secondary_look_priority_override(_exact_facing) path (force-stop facing lock) */
        facing_succeeded = 0;
        set_exact_facing = stimulus_class == _secondary_look_priority_override_exact_facing;
        exact_facing_value = set_exact_facing;
        if (actor->control.free_facing_vector)
        {
LABEL_94:
            set_exact_facing = 1;
            goto LABEL_95;
        }
        if (!actor_look_valid_aim_vector(aim_yaw_deviation, &actor->control.desired_facing_vector, spec_vector))
        {
            if (!actor_move_force_stop(actor_index))
                goto LABEL_96;
            exact_facing_value = 1;
            goto LABEL_94;
        }
LABEL_95:
        facing_succeeded = 1;
LABEL_96:
        if (facing_succeeded)
        {
            if (set_exact_facing)
            {
                actor->control.face_exactly = exact_facing_value;
                actor->control.desired_facing_vector = *spec_vector;
            }
            goto LABEL_99;
        }

LABEL_103:
        look_class4 = look_class;
        if (look_class == _primary_priority_opportunity_aiming && facing_available
            && actor_look_valid_aim_vector(aim_yaw_deviation, &actor->control.desired_facing_vector, &decoded))
        {
            actor->control.desired_aiming_vector = decoded;
            if (can_aim)
                actor->control.desired_looking_vector = decoded;
            facing_available = 0;
            actor->control.aiming_away_from_primary = 0;
        }

        /* DEVIATION: inlined verbatim copy of actor_look_get_looking_definition@0x837FB1E8 (0 code xrefs) collapsed to a call; actor_index passes straight through unfolded, no dead-guard args to fold. */
        /* layer 4: idle looking — pick the looking-mode substruct from the variant definition */
        idle = actor_look_get_looking_definition(actor_index);

        has_idle_aim = idle->idle_aim_time_upper_bound > 0.0;
        has_idle_look = idle->idle_look_time_upper_bound > 0.0;

        if (actor->orders.look.idle_look_type <= 0
            || look_applied
            || !facing_available && !can_aim
            || idle->idle_facing_time_upper_bound <= 0.0
            && idle->idle_aim_time_upper_bound <= 0.0
            && idle->idle_look_time_upper_bound <= 0.0)
        {
            actor_index_copy = actor_index;
            actor->control.idle_major_active = 0;
            actor->control.idle_major_direction_is_interesting = 0;
LABEL_179:
            actor->control.idle_minor_active = 0;
            goto LABEL_180;
        }

        idle_updated = 0;
        idle_major_new = 0;
        free_facing = idle->idle_facing_time_upper_bound > 0.0
                   && aim_pending
                   && look_class4 == _primary_priority_face_360
                   && !actor->control.idle_facing_timer;
        facing_timer = actor->control.idle_facing_timer;
        if (facing_timer > 0)
            actor->control.idle_facing_timer = facing_timer - 1;

        if (actor->control.idle_major_active && actor->control.idle_major_is_aiming && !facing_available)
        {
            actor->control.idle_major_active = 1;
            actor_index_copy = actor_index;
            actor->control.idle_major_timer = actor_look_idle_timer(actor_index, idle, 2, 1u);
            actor->control.idle_major_direction.type = _direction_specification_vector;
            actor->control.idle_major_direction.___u1.vector = actor->control.desired_aiming_vector;
        }
        else
        {
            actor_index_copy = actor_index;
        }

        if (!actor->control.idle_major_active || !actor->control.idle_major_timer)
        {
            major_is_aiming = 1;
            major_candidate_ok = facing_available && has_idle_aim;
            if (!major_candidate_ok)
            {
                major_is_aiming = 0;
                major_candidate_ok = can_aim && has_idle_look;
            }
            if (major_candidate_ok)
            {
                minor_has_variation = major_is_aiming && can_aim && has_idle_look;
                base_vector = &actor->control.desired_facing_vector;
                if (!major_is_aiming)
                    base_vector = &actor->control.desired_aiming_vector;
                actor->control.idle_major_direction_is_interesting = actor_look_idle_new_major_direction(actor_index_copy, idle, base_vector, free_facing, major_is_aiming, minor_has_variation);
                idle_major_new = 1;
            }
        }

        if (actor->control.idle_major_active)
        {
            --actor->control.idle_major_timer;
            if (actor_look_decode_direction(actor_index_copy, &actor->control.idle_major_direction, spec_vector))
            {
                if (facing_available)
                {
                    if (free_facing)
                    {
                        idle_updated = 1;
                        actor->control.aiming_away_from_primary = 1;
                        actor->control.desired_facing_vector = *spec_vector;
                        actor->control.desired_aiming_vector = *spec_vector;
                    }
                    else
                    {
                        idle_updated = actor_look_valid_aim_vector(aim_yaw_deviation, &actor->control.desired_facing_vector, spec_vector);
                        if (idle_updated)
                        {
                            actor->control.aiming_away_from_primary = 1;
                            actor->control.desired_aiming_vector = *spec_vector;
                        }
                    }
                }
                else
                {
                    idle_updated = actor_look_valid_look_vector(look_yaw_deviation, &look_cone_cosines,
                                                                (const real_vector2d *)&actor->control.desired_facing_vector,
                                                                (const real_vector2d *)&actor->control.desired_aiming_vector,
                                                                (const real_vector2d *)spec_vector);
                    if (idle_updated)
                        actor->control.desired_looking_vector = *spec_vector;
                }
            }
            if (idle_updated)
            {
                if (idle_major_new)
                {
                    if (has_idle_look)
                    {
                        actor->control.idle_minor_active = 1;
                        actor->control.idle_minor_timer = actor_look_idle_timer(actor_index_copy, idle, 2, actor->control.idle_major_direction_is_interesting);
                        actor->control.idle_minor_direction = actor->control.idle_major_direction;
                        if (free_facing)
                            actor->control.idle_facing_timer = actor_look_idle_timer(actor_index_copy, idle, 0, actor->control.idle_major_direction_is_interesting);
                    }
                }
            }
        }

        if (!idle_updated)
        {
            actor->control.idle_major_active = 0;
            *spec_vector = actor->control.desired_aiming_vector;
        }

        if (!facing_available || !can_aim || !has_idle_look)
            goto LABEL_179;
        if (!actor->control.idle_minor_timer)
            actor_look_idle_new_minor_direction(actor_index_copy, idle, spec_vector);
        minor_active = actor->control.idle_minor_active;
        --actor->control.idle_minor_timer;
        if (!minor_active)
            goto LABEL_180;
        {
            uint8_t minor_ok = 0;
            if (actor_look_decode_direction(actor_index_copy, &actor->control.idle_minor_direction, &decoded))
                minor_ok = actor_look_valid_look_vector(look_yaw_deviation, &look_cone_cosines,
                                                        (const real_vector2d *)&actor->control.desired_facing_vector,
                                                        (const real_vector2d *)&actor->control.desired_aiming_vector,
                                                        (const real_vector2d *)&decoded);
            if (!minor_ok)
                goto LABEL_179;
            actor->control.desired_looking_vector = decoded;
        }

LABEL_180:
        /* aim/look cross-validation fallback for non-busy, untargeted units: if the desired aim is
         * reachable from the desired facing but not from the unit's actual facing (and likewise for
         * looking), force exact facing so the body turns */
        if (!actor->control.moving && !actor->control.moving_forced_by_aiming)
        {
            if (!unit_is_busy(actor->meta.unit_index) && actor->input.vehicle_index == -1)
            {
                if (actor_look_valid_aim_vector(aim_yaw_deviation, &actor->control.desired_facing_vector, &actor->control.desired_aiming_vector)
                    && !actor_look_valid_aim_vector(aim_yaw_deviation, &actor->input.facing_vector, &actor->control.desired_aiming_vector)
                    || can_aim_saved
                    && actor_look_valid_look_vector(look_yaw_deviation, &look_cone_cosines,
                                                    (const real_vector2d *)&actor->control.desired_facing_vector,
                                                    (const real_vector2d *)&actor->control.desired_aiming_vector,
                                                    (const real_vector2d *)&actor->control.desired_looking_vector)
                    && !actor_look_valid_look_vector(look_yaw_deviation, &look_cone_cosines,
                                                     (const real_vector2d *)&actor->input.facing_vector,
                                                     (const real_vector2d *)&actor->control.desired_aiming_vector,
                                                     (const real_vector2d *)&actor->control.desired_looking_vector))
                {
                    actor->control.face_exactly = 1;
                }
            }
        }

        if (!can_aim_saved)
            actor->control.desired_looking_vector = actor->control.desired_aiming_vector;

LABEL_192:
        /* zero out a tiny facing-Z residual; if the facing flattens to nothing, fall back to body facing */
        if (!actor->state.flying && __fabs(actor->control.desired_facing_vector.n[2]) >= 0.000099999997)
        {
            actor->control.desired_facing_vector.n[2] = 0.0;
            if (normalize2d((real_vector2d *)&actor->control.desired_facing_vector) == 0.0)
                actor->control.desired_facing_vector = actor->input.facing_vector;
        }

        /* stationary-facing lock maintenance */
        if (actor->control.desire_stationary_facing)
        {
            if (actor->control.fixed_stationary_facing)
            {
                stationary_angle = actor_def->defensive.stationary_facing_angle;
                if (stationary_angle > 0.0f)
                {
                    stationary_cos = (float)cos(stationary_angle);
                    if (actor->state.flying)
                    {
                        if (((actor->control.fixed_stationary_facing_vector.n[0] * actor->control.desired_facing_vector.n[0])
                                  + ((actor->control.desired_facing_vector.n[1] * actor->control.fixed_stationary_facing_vector.n[1])
                                          + (actor->control.desired_facing_vector.n[2] * actor->control.fixed_stationary_facing_vector.n[2]))) <= stationary_cos
                            || (stationary_ok = 1,
                                ((actor->control.fixed_stationary_facing_vector.n[0] * actor->control.desired_aiming_vector.n[0])
                                      + ((actor->control.desired_aiming_vector.n[1] * actor->control.fixed_stationary_facing_vector.n[1])
                                              + (actor->control.desired_aiming_vector.n[2] * actor->control.fixed_stationary_facing_vector.n[2]))) <= stationary_cos))
                        {
                            stationary_ok = 0;
                        }
                    }
                    else
                    {
                        stationary_facing2d = *(const real_vector2d *)actor->control.desired_facing_vector.n;
                        stationary_aiming2d = *(const real_vector2d *)actor->control.desired_aiming_vector.n;
                        stationary_fixed2d = *(const real_vector2d *)actor->control.fixed_stationary_facing_vector.n;
                        if (normalize2d(&stationary_facing2d) == 0.0
                            || normalize2d(&stationary_aiming2d) == 0.0
                            || normalize2d(&stationary_fixed2d) == 0.0)
                        {
                            stationary_ok = 0;
                        }
                        else if (((stationary_fixed2d.n[0] * stationary_facing2d.n[0]) + (stationary_facing2d.n[1] * stationary_fixed2d.n[1])) <= stationary_cos
                              || (stationary_ok = 1,
                                  ((stationary_fixed2d.n[0] * stationary_aiming2d.n[0]) + (stationary_aiming2d.n[1] * stationary_fixed2d.n[1])) <= stationary_cos))
                        {
                            stationary_ok = 0;
                        }
                    }
                    if (!stationary_ok)
                    {
                        actor->control.fixed_stationary_facing = 0;
                        actor_stimulus_abandon_stationary_facing(actor_index_copy);
                    }
                }
            }
            else if (!actor->control.moving
                   && ((actor->input.aiming_vector.n[0] * actor->control.desired_aiming_vector.n[0])
                            + ((actor->input.aiming_vector.n[2] * actor->control.desired_aiming_vector.n[2])
                                    + (actor->input.aiming_vector.n[1] * actor->control.desired_aiming_vector.n[1]))) > 0.89999998)
            {
                actor->control.fixed_stationary_facing = 1;
                actor->control.fixed_stationary_facing_vector = actor->control.desired_facing_vector;
            }
        }
        else
        {
            actor->control.fixed_stationary_facing = 0;
        }

        /* publish the resolved facing/aiming/looking vectors to the output slots */
        actor->output.facing_vector = actor->control.desired_facing_vector;
        actor->output.aiming_vector = actor->control.desired_aiming_vector;
        actor->output.looking_vector = actor->control.desired_looking_vector;
        actor_unit_control_exact_facing(actor_index_copy, actor->control.face_exactly);

        if (look_actively_engaged || actor->orders.look.idle_look_type == _idle_look_combat)
        {
LABEL_221:
            actor->output.aiming_speed = 0;
        }
        else
        {
            switch ((uint16_t)actor->control.secondary_look_type)
            {
                case _secondary_look_weapon_impact:
                case _secondary_look_detonation:
                case _secondary_look_combat_stimulus_prop:
                case _secondary_look_damage:
                case _secondary_look_dangerous_object:
                    goto LABEL_221;
                default:
                    actor->output.aiming_speed = 1;
                    break;
            }
        }
    }
}
