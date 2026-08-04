/* actor_look_update @0x837FBA10 — resolves an actor's desired facing / aiming / looking vectors
 * for the frame and publishes them to the unit-control output slots.
 *
 * The function layers candidate look directions in priority order:
 *   1. firing-burst aim (forces the aim vector while a burst is in progress)
 *   2. an explicit look request (actor word500/word502 look state)
 *   3. a stimulus-driven look (actor word674..676)
 *   4. idle looking (timed major/minor wandering of facing/aim/look)
 *   5. a stationary-facing lock and the final aim->look fallback copy
 * Each layer validates its candidate with actor_look_valid_aim_vector / _valid_look_vector
 * (cone tests around the actor facing) before committing it.
 *
 * Reconstructed from the decompiler. The goto control flow, every validation call, and all flag
 * side-effects are preserved verbatim so branch semantics are not silently altered; PPC `_cntlzw` bit
 * tests are reduced to the equality comparisons they implement (marked inline). Several decompiler
 * scratch values (angle_scratch, scratch_look_vector) are used before assignment by the original codegen — preserved with comments.
 *
 * DE-DUP PASS (2026-07-15): the decompiler rendered every vector move as int/__int64 bit-copy triples
 * (`*(int *)&v.n[0] = ...; *(__int64 *)&v.n[1] = ...`). Each was verified against the disassembly to be
 * a whole-`real_vector3d` copy and rewritten as a struct assignment (`dst = src`), and the ~35 scratch
 * locals they needed were removed. The remaining Hex-Rays idioms (HIBYTE(look_seed), spec_tail/HIDWORD
 * in the stimulus-commit block, LODWORD/__int128 in the stationary test) sit inside goto-interleaved or
 * offset-punned code and were left byte-identical rather than risk altering control flow.
 */

#include <stdint.h>
#include "headers/hexrays_defs.h" /* HIBYTE/LODWORD/HIDWORD word-extraction macros */
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_idle_looking.h"
#include "headers/direction_specification.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/actor_mode.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/blam_data_globals.h"
extern double cos(double x);
extern double __fabs(double x);

/* stale inline extern for global_tag_instances removed; the canonical decl comes
 * from headers/blam_data_globals.h (fixes C2371). */

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
    /* DEVIATION: the decompiler promoted this scratch accumulator to a phantom 2nd parameter.
     * Disasm + DB prototype confirm a single r3 param; angle_scratch is written before every read
     * (see 0x837FBB2C/0x837FC630 stores), so it is a local, not an incoming argument. */
    long double angle_scratch;
    actor_datum *actor;
    char look_enabled;
    __int16 unit_action;
    /* DB: the tag at meta.definition_index is an actor_definition (same tag re-read
     * as actor_definition at def_base below); every offset resolves to a named
     * actor_definition member via res.py. Retyped from the decompiler's float*. */
    const actor_definition *actor_def;
    bool can_aim;
    int actor_index_copy;
    double aim_yaw_deviation;
    double look_yaw_deviation;
    char aim_pending;
    char look_pending;
    char facing_available;
    char aim_applied;
    char look_applied;
    long double cos_yaw;
    double pitch_cos_arg;
    long double cos_pitch;
    char burst_direction_valid;
    real_vector3d *valid;
    direction_specification *empty_dir;
    int empty_prop_index;
    float empty_point_y, empty_point_z;
    __int16 look_class;
    __int16 look_mode;
    __int16 stimulus_class;
    __int16 clamped;
    int look_timer;
    __int16 next_timer;
    int look_class_snapshot;
    char aim_succeeded;
    char tmp_flag_a, tmp_flag_b, tmp_flag_c;
    bool zf;
    float spec_y;
    unsigned __int64 spec_tail;
    int spec_type;
    char facing_succeeded;
    bool set_exact_facing;
    char exact_facing_value;
    /* def_index/looking_mode/def_base retired: their block collapsed into
     * actor_look_get_looking_definition() (see the DEVIATION below). */
    actor_idle_looking *idle;
    bool has_idle_aim;
    bool has_idle_look;
    unsigned __int8 idle_updated;
    char idle_major_new;
    char free_facing_tmp;
    unsigned __int8 free_facing;
    int facing_timer;
    unsigned __int8 major_is_aiming;
    char idle_test_tmp;
    unsigned __int8 minor_test_tmp;
    const real_vector3d *base_vector;
    real_vector3d *decode_major;
    int look_class4;
    unsigned __int8 minor_valid;
    float *decode_minor;
    real_vector3d *unit_busy;
    real_vector3d *scratch_look_vector;          /* used before assignment by original codegen (decompiler artifact) */
    real_vector3d *aim_test2;
    float *look_test2;
    long double scratch;
    double stationary_cos;
    char stationary_ok;
    /* was __int128 (unsupported on PPC target); a 16-byte scratch buffer the decompiler
     * fills with overlapping 8-byte stores and reads back as a real_vector2d. Byte-exact. */
    union { real_vector2d vec; unsigned char raw[16]; } stationary_pack;
    bool can_aim_saved;
    unsigned __int8 look_actively_engaged; /* v116: decompiler packed this "look actively engaged" flag into a byte of a __int16 stack slot */
    real_vector3d facing_basis;          /* v118 */
    direction_specification spec;        /* v119 */
    real_vector3d decoded;               /* v120 */
    direction_specification empty_storage; /* v121 */
    real_vector2d stationary_dir2d;                  /* scratch for the stationary-facing test (assigned at use) */

    actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    look_enabled = 1;
    look_actively_engaged = 0;
    unit_action = actor->output.movement_type;
    actor_def = TAG_GET(const actor_definition, actor->meta.definition_index);
    /* (removed the dead `stationary_dir2d.n[0] = actor_def` alias store: it only
     * existed so the decompiler could read actor_def[204] through stationary_dir2d;
     * we now read the typed member directly, and stationary_dir2d is freshly
     * assigned before its real use in the stationary test.) */

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

    /* facing basis vector from the variant's yaw/pitch (firing pose vs. normal pose) */
    {
        if (actor->state.mode == _actor_mode_combat)
        {
            *(double *)&angle_scratch = actor_def->looking.combat_looking_delta_angles[0];
            cos_yaw = cos(angle_scratch);
            pitch_cos_arg = actor_def->looking.combat_looking_delta_angles[1];
        }
        else
        {
            *(double *)&angle_scratch = actor_def->looking.noncombat_looking_delta_angles[0];
            cos_yaw = cos(angle_scratch);
            pitch_cos_arg = actor_def->looking.noncombat_looking_delta_angles[1];
        }
        facing_basis.n[0] = *(double *)&cos_yaw;
        *(double *)&cos_yaw = pitch_cos_arg;
        cos_pitch = cos(cos_yaw);
        facing_basis.n[1] = *(double *)&cos_pitch;
        burst_direction_valid = 0;

        /* layer 1: firing-burst aim */
        valid = (real_vector3d *)actor_combat_currently_firing_burst(actor_index);
        if (!(char)valid)
            goto LABEL_18;
        if (actor->orders.combat.abort_burst)
            goto LABEL_18;
        empty_dir = direction_get_empty(&empty_storage);
        empty_prop_index = empty_dir->___u1.prop_index;
        empty_point_y = empty_dir->___u1.point.n[1];
        empty_point_z = empty_dir->___u1.point.n[2];
        *(int *)&spec.type = *(int *)&empty_dir->type;
        spec.___u1.prop_index = empty_prop_index;
        spec.___u1.point.n[1] = empty_point_y;
        spec.___u1.point.n[2] = empty_point_z;
        spec.type = _direction_specification_target;
        valid = (real_vector3d *)actor_look_decode_direction(actor_index, &spec, &decoded);
        if ((char)valid)
        {
            look_class = _primary_priority_locked_aiming;
            look_actively_engaged = 1;
            burst_direction_valid = 1;
        }
        else
        {
LABEL_18:
            look_class = _primary_priority_none; /* look_seed slot is 0 on every path reaching here */
        }

        /* layer 2: explicit look state (actor word500/word502) */
        if (!burst_direction_valid)
        {
            look_mode = actor->orders.look.primary_priority;
            look_class = look_mode;
            if (look_mode)
            {
                if (look_mode != _primary_priority_face_360)
                {
                    valid = (real_vector3d *)actor_look_decode_direction(actor_index, &actor->orders.look.primary_direction, &decoded);
                    if ((char)valid)
                        look_actively_engaged = (actor->orders.look.primary_direction.type == _direction_specification_target); /* (_cntlzw(x-2)&0x20)!=0  ==  (x==2) */
                    else
                        look_class = _primary_priority_none;
                }
            }
        }

        /* layer 3: stimulus look (actor word674..676) */
        stimulus_class = _secondary_look_priority_none;
        if ((unsigned __int16)actor->control.secondary_look_type < 0x8000u && actor->control.secondary_look_timer > 0)
        {
            valid = (real_vector3d *)actor_look_decode_direction(actor_index, &actor->control.secondary_look_direction, (real_vector3d *)&spec);
            if ((char)valid)
                stimulus_class = actor->control.secondary_look_priority;
        }
        if (actor->control.moving)
        {
            valid = (real_vector3d *)actor_action_class(actor_index);
            if ((__int16)valid == 2)
            {
                clamped = stimulus_class;
                if (stimulus_class > _secondary_look_priority_turn_and_aim)
                    clamped = _secondary_look_priority_turn_and_aim;
                stimulus_class = clamped;
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
                && (valid = (real_vector3d *)actor_look_valid_aim_vector(aim_yaw_deviation, valid, &actor->control.desired_facing_vector), !(char)valid))
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
                tmp_flag_a = actor->control.face_exactly;
                aim_pending = 0;
                actor->control.desired_facing_vector = decoded;
                look_pending = 0;
                actor->control.face_exactly = (look_class == _primary_priority_exact_facing) | tmp_flag_a; /* (_cntlzw(x-4)&0x20)!=0 == (x==4) */
            }
            if (!actor->control.free_facing_vector && !actor->control.optional_facing_vector || (tmp_flag_b = 0, look_class >= _primary_priority_locked_facing))
                tmp_flag_b = 1;
            aim_applied = tmp_flag_b;
        }

        /* commit stimulus-class look (layer 3 resolution) */
        /* range idiom: secondary priorities idle_look(2)..stop_and_aim(6); override(7)/override_exact_facing(8) fall through to LABEL_103 / the facing-lock path below */
        if ((unsigned int)(stimulus_class - _secondary_look_priority_idle_look) > 6)
            goto LABEL_103;
        if (stimulus_class == _secondary_look_priority_idle_aim || stimulus_class == _secondary_look_priority_aim
            || stimulus_class == _secondary_look_priority_turn_and_aim || stimulus_class == _secondary_look_priority_stop_and_aim
            || stimulus_class == _secondary_look_priority_idle_look)
        {
            valid = (real_vector3d *)actor_look_valid_aim_vector(aim_yaw_deviation, valid, &actor->control.desired_facing_vector);
            zf = aim_applied != 0;
            tmp_flag_c = (char)valid;
            if (!zf && !look_applied)
            {
                if (stimulus_class >= _secondary_look_priority_stop_and_aim && (valid = (real_vector3d *)actor_move_force_stop(actor_index), (char)valid)
                    || stimulus_class >= _secondary_look_priority_turn_and_aim && (look_pending || actor->control.free_facing_vector)
                    || stimulus_class >= _secondary_look_priority_aim && !tmp_flag_c && aim_pending && facing_available)
                {
                    spec_y = spec.___u1.point.n[1];
                    spec_tail = *(__int64 *)&spec.type;
                    if (!actor->control.face_exactly || !tmp_flag_c)
                    {
                        *(int *)&actor->control.desired_facing_vector.n[0] = *(int *)&spec.type;
                        *(int *)&actor->control.desired_facing_vector.n[1] = spec_tail;
                        actor->control.desired_facing_vector.n[2] = spec_y;
                        actor->control.face_exactly = 0;
                    }
LABEL_99:
                    aim_pending = 0;
                    actor->control.desired_aiming_vector.n[2] = spec_y;
                    *(int *)&actor->control.desired_aiming_vector.n[0] = (unsigned int)(spec_tail >> 32);
                    actor->control.desired_looking_vector.n[2] = spec_y;
                    *(int *)&actor->control.desired_looking_vector.n[0] = (unsigned int)(spec_tail >> 32);
                    goto LABEL_100;
                }
            }
            if (!look_applied && tmp_flag_c && (stimulus_class >= _secondary_look_priority_turn_and_aim || stimulus_class >= _secondary_look_priority_idle_aim && facing_available))
            {
                spec_type = *(int *)&spec.type;
                spec_tail = ((unsigned __int64)(*(int *)&spec.___u1.point.n[1]) << 32) | (unsigned int)spec.___u1.prop_index;
                *(int *)&actor->control.desired_looking_vector.n[0] = *(int *)&spec.type;
                *(int *)&actor->control.desired_looking_vector.n[2] = (unsigned int)(spec_tail >> 32);
                *(int *)&actor->control.desired_aiming_vector.n[0] = spec_type;
                *(int *)&actor->control.desired_aiming_vector.n[2] = (unsigned int)(spec_tail >> 32);
LABEL_100:
                look_applied = 0;
                actor->control.aiming_away_from_primary = 1;
                goto LABEL_101;
            }
            if (can_aim)
            {
                valid = (real_vector3d *)actor_look_valid_look_vector(look_yaw_deviation, (const real_vector2d *)valid->n, (const real_vector2d *)&facing_basis,
                                                                      (const real_vector2d *)&actor->control.desired_facing_vector, (const real_vector2d *)&actor->control.desired_aiming_vector);
                if ((char)valid)
                {
                    can_aim = 0;
                    spec_tail = ((unsigned __int64)(*(int *)&spec.___u1.point.n[1]) << 32) | (unsigned int)spec.___u1.prop_index;
                    *(int *)&actor->control.desired_looking_vector.n[0] = *(int *)&spec.type;
                    *(int *)&actor->control.desired_looking_vector.n[2] = (unsigned int)(spec_tail >> 32);
LABEL_102:
                    *(int *)&actor->control.desired_looking_vector.n[1] = spec_tail;
                    goto LABEL_103;
                }
            }
            if (facing_available && tmp_flag_c)
            {
                int spec_type2 = *(int *)&spec.type;
                can_aim = 1;
                spec_tail = ((unsigned __int64)(*(int *)&spec.___u1.point.n[1]) << 32) | (unsigned int)spec.___u1.prop_index;
                *(int *)&actor->control.desired_looking_vector.n[0] = *(int *)&spec.type;
                *(int *)&actor->control.desired_looking_vector.n[2] = (unsigned int)(spec_tail >> 32);
                *(int *)&actor->control.desired_aiming_vector.n[0] = spec_type2;
                *(int *)&actor->control.desired_aiming_vector.n[2] = (unsigned int)(spec_tail >> 32);
LABEL_101:
                *(int *)&actor->control.desired_aiming_vector.n[1] = spec_tail;
                facing_available = 0;
                goto LABEL_102;
            }
            goto LABEL_103;
        }

        /* _secondary_look_priority_override_exact_facing (8) path (force-stop facing lock) */
        facing_succeeded = 0;
        set_exact_facing = stimulus_class == _secondary_look_priority_override_exact_facing;
        exact_facing_value = set_exact_facing;
        if (actor->control.free_facing_vector)
        {
LABEL_94:
            set_exact_facing = 1;
            goto LABEL_95;
        }
        valid = (real_vector3d *)actor_look_valid_aim_vector(aim_yaw_deviation, valid, &actor->control.desired_facing_vector);
        if (!(char)valid)
        {
            valid = (real_vector3d *)actor_move_force_stop(actor_index);
            if (!(char)valid)
                goto LABEL_96;
            exact_facing_value = 1;
            goto LABEL_94;
        }
LABEL_95:
        facing_succeeded = 1;
LABEL_96:
        if (facing_succeeded)
        {
            spec_y = spec.___u1.point.n[1];
            spec_tail = *(__int64 *)&spec.type;
            if (set_exact_facing)
            {
                actor->control.face_exactly = exact_facing_value;
                *(__int64 *)&actor->control.desired_facing_vector.n[0] = spec_tail;
                actor->control.desired_facing_vector.n[2] = spec_y;
            }
            goto LABEL_99;
        }

LABEL_103:
        look_class4 = look_class;
        if (look_class == _primary_priority_opportunity_aiming && facing_available && actor_look_valid_aim_vector(aim_yaw_deviation, valid, &actor->control.desired_facing_vector))
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
        if (idle->idle_facing_time_upper_bound <= 0.0 || !aim_pending || look_class4 != _primary_priority_face_360 || (free_facing_tmp = 1, actor->control.idle_facing_timer))
            free_facing_tmp = 0;
        free_facing = free_facing_tmp;
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
            if (!facing_available || (idle_test_tmp = 1, !has_idle_aim))
                idle_test_tmp = 0;
            if (!idle_test_tmp)
            {
                major_is_aiming = 0;
                if (!can_aim || (idle_test_tmp = 1, !has_idle_look))
                    idle_test_tmp = 0;
            }
            if (idle_test_tmp)
            {
                if (!major_is_aiming || !can_aim || (minor_test_tmp = 1, !has_idle_look))
                    minor_test_tmp = 0;
                base_vector = (const real_vector3d *)&actor->control.desired_facing_vector;
                if (!major_is_aiming)
                    base_vector = (const real_vector3d *)&actor->control.desired_aiming_vector;
                actor->control.idle_major_direction_is_interesting = actor_look_idle_new_major_direction(actor_index_copy, idle, base_vector, free_facing, major_is_aiming, minor_test_tmp);
                idle_major_new = 1;
            }
        }

        if (actor->control.idle_major_active)
        {
            --actor->control.idle_major_timer;
            decode_major = (real_vector3d *)actor_look_decode_direction(actor_index_copy, &actor->control.idle_major_direction, (real_vector3d *)&spec);
            if ((char)decode_major)
            {
                /* decode_major wrote the decoded direction into spec's storage (see decode call above) */
                if (facing_available)
                {
                    if (free_facing)
                    {
                        idle_updated = 1;
                        actor->control.aiming_away_from_primary = 1;
                        actor->control.desired_facing_vector = *(real_vector3d *)&spec;
                        actor->control.desired_aiming_vector = *(real_vector3d *)&spec;
                    }
                    else
                    {
                        idle_updated = actor_look_valid_aim_vector(aim_yaw_deviation, decode_major, &actor->control.desired_facing_vector);
                        if (idle_updated)
                        {
                            actor->control.aiming_away_from_primary = 1;
                            actor->control.desired_aiming_vector = *(real_vector3d *)&spec;
                        }
                    }
                }
                else
                {
                    idle_updated = actor_look_valid_look_vector(look_yaw_deviation, (const real_vector2d *)decode_major->n, (const real_vector2d *)&facing_basis,
                                                                (const real_vector2d *)&actor->control.desired_facing_vector, (const real_vector2d *)&actor->control.desired_aiming_vector);
                    if (idle_updated)
                        actor->control.desired_looking_vector = *(real_vector3d *)&spec;
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
            int r364 = *(int *)&actor->control.desired_aiming_vector.n[0];
            int r365 = *(int *)&actor->control.desired_aiming_vector.n[1];
            float r366 = actor->control.desired_aiming_vector.n[2];
            actor->control.idle_major_active = 0;
            *(int *)&spec.type = r364;
            spec.___u1.prop_index = r365;
            spec.___u1.point.n[1] = r366;
        }

        if (!facing_available || !can_aim || !has_idle_look)
            goto LABEL_179;
        if (!actor->control.idle_minor_timer)
            actor_look_idle_new_minor_direction(actor_index_copy, idle, (const real_vector3d *)&spec);
        minor_valid = actor->control.idle_minor_active;
        --actor->control.idle_minor_timer;
        if (!minor_valid)
            goto LABEL_180;
        {
            unsigned __int8 minor_ok = 0;
            decode_minor = (float *)actor_look_decode_direction(actor_index_copy, &actor->control.idle_minor_direction, &decoded);
            if ((char)decode_minor)
                minor_ok = actor_look_valid_look_vector(look_yaw_deviation, (const real_vector2d *)decode_minor, (const real_vector2d *)&facing_basis,
                                                        (const real_vector2d *)&actor->control.desired_facing_vector, (const real_vector2d *)&actor->control.desired_aiming_vector);
            if (!minor_ok)
                goto LABEL_179;
            actor->control.desired_looking_vector = decoded;
        }

LABEL_180:
        /* aim/look cross-validation fallback for non-busy, untargeted units */
        if (!actor->control.moving && !actor->control.moving_forced_by_aiming)
        {
            unit_busy = (real_vector3d *)unit_is_busy(actor->meta.unit_index);
            if (!(char)unit_busy && actor->input.vehicle_index == -1)
            {
                if ((aim_test2 = (real_vector3d *)actor_look_valid_aim_vector(aim_yaw_deviation, unit_busy, &actor->control.desired_facing_vector), (char)aim_test2)
                    && (aim_test2 = (real_vector3d *)actor_look_valid_aim_vector(*(double *)&scratch, aim_test2, &actor->input.facing_vector), !(char)aim_test2)
                    || can_aim_saved
                    && (look_test2 = (float *)actor_look_valid_look_vector(look_yaw_deviation, (const real_vector2d *)aim_test2->n, (const real_vector2d *)&facing_basis, (const real_vector2d *)scratch_look_vector, (const real_vector2d *)&actor->control.desired_aiming_vector), (char)look_test2)
                    && !actor_look_valid_look_vector(look_yaw_deviation, (const real_vector2d *)look_test2, (const real_vector2d *)&facing_basis, (const real_vector2d *)&actor->input.facing_vector, (const real_vector2d *)&actor->control.desired_aiming_vector))
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
                /* offset 816 = actor_def->defensive.stationary_facing_angle; the
                 * decompiler's *(double*)&angle_scratch reinterpret was PPC long-double
                 * noise around a plain float load — simplified to a direct assignment. */
                angle_scratch = actor_def->defensive.stationary_facing_angle;
                if (angle_scratch > 0.0)
                {
                    stationary_cos = cos(angle_scratch);
                    stationary_cos = (float)*&stationary_cos;
                    if (actor->state.flying)
                    {
                        if (((actor->control.fixed_stationary_facing_vector.n[0] * actor->control.desired_facing_vector.n[0])
                                  + ((actor->control.desired_facing_vector.n[1] * actor->control.fixed_stationary_facing_vector.n[1])
                                          + (actor->control.desired_facing_vector.n[2] * actor->control.fixed_stationary_facing_vector.n[2]))) <= (float)stationary_cos
                            || (stationary_ok = 1,
                                ((actor->control.fixed_stationary_facing_vector.n[0] * actor->control.desired_aiming_vector.n[0])
                                      + ((actor->control.desired_aiming_vector.n[1] * actor->control.fixed_stationary_facing_vector.n[1])
                                              + (actor->control.desired_aiming_vector.n[2] * actor->control.fixed_stationary_facing_vector.n[2]))) <= (float)stationary_cos))
                        {
                            stationary_ok = 0;
                        }
                    }
                    else
                    {
                        /* RAW (Category-B): stationary_pack is a local scratch buffer the decompiler fills with
                         * overlapping 8-byte stores to repack three 3-float vectors into real_vector2d pairs; the
                         * +4 byte offsets are that overlap, not a struct member. Kept byte-exact. */
                        *((__int64 *)&stationary_pack + 1) = *(__int64 *)&actor->control.desired_facing_vector.n[0];
                        *(__int64 *)((char *)&stationary_pack + 4) = *(__int64 *)&actor->control.desired_aiming_vector.n[0];
                        *(__int64 *)&stationary_pack = *(__int64 *)&actor->control.fixed_stationary_facing_vector.n[0];
                        stationary_dir2d = stationary_pack.vec; /* was (real_vector2d)__int128: low 8 bytes */
                        *(__int64 *)&spec.type = *(__int64 *)((char *)&stationary_pack + 4);
                        *(__int64 *)facing_basis.n = *(__int64 *)&stationary_pack;
                        if (normalize2d(&stationary_dir2d) == 0.0
                            || normalize2d((real_vector2d *)&spec) == 0.0
                            || normalize2d((real_vector2d *)&facing_basis) == 0.0)
                        {
                            stationary_ok = 0;
                        }
                        else if (((facing_basis.n[0] * stationary_dir2d.n[0]) + (stationary_dir2d.n[1] * facing_basis.n[1])) <= (float)stationary_cos
                              || (stationary_ok = 1,
                                  ((*(float *)&spec.type * facing_basis.n[0]) + (spec.___u1.point.n[0] * facing_basis.n[1])) <= (float)stationary_cos))
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
            switch ((unsigned __int16)actor->control.secondary_look_type)
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
