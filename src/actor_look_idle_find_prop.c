/* actor_look_idle_find_prop @0x837FAF08 — while idle, have an actor pick the most interesting nearby "prop"
 * (a point/object of interest) to glance at. Each prop in the actor's prop list is scored by its base interest
 * (prop+84) scaled by a freshness factor that decays as the prop's look timer (prop+88) runs down and recovers
 * the longer it has been since it was last looked at (prop+92, in 1/600s ticks). Props of the wrong type, with
 * no interest, or outside the actor's look cone / aim cone are skipped. The highest-scoring prop's index is
 * written into *direction (as a type-1 prop spec), its timer is refreshed, and *interesting_prop records
 * whether the chosen prop is newly interesting (its timer hadn't yet decayed). Returns 1 if a prop was chosen.
 *
 * DEVIATION 1: the look-cone vector and the prop_iterator share a stack slot in the original (the iterator
 * begins at the vector's third component); reproduced with an overlaid struct so the iterator's current prop
 * index is exactly what is passed through as facing_vector.n[2], matching the binary.
 * DEVIATION 2: cos()'s float arg shadows a GPR slot (FPR-shadow); the decompiler's long-double scratch locals
 * are dropped in favour of plain cos(angle) calls. The freshness factor's int->float conversions used hardware
 * fcfid, which the decompiler rendered as __int64/magic-constant arithmetic; restored as plain (float) casts. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/prop_iterator.h"
#include "headers/actor_definition.h"
#include "headers/actor_mode.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
/* extern corrected to match def (actor_look_valid_look_vector.c): 5 params in def order —
 * (double yaw, cone_limits, aiming_vector, attempted_looking_vector, look_vector), returns int.
 * The old extern carried the decompiler's phantom r3 param and had the vectors mis-ordered. */
extern uint8_t actor_look_valid_look_vector(float yaw_deviation, const real_vector2d *cone_limits, const real_vector2d *aiming_vector, const real_vector2d *attempted_looking_vector, const real_vector2d *look_vector);

extern double cos(double angle);
extern float __fsqrts(float x);
extern double __fabs(double x);

uint8_t actor_look_idle_find_prop(int actor_index, uint8_t free_facing, uint8_t aim_at_prop, direction_specification *direction, uint8_t *interesting_prop)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);
    int game_time = game_time_get();

    float look_cone_threshold = actor_def->looking.runtime_maximum_aiming_deviation_cosine.n[0];  /* min cos for a prop inside the aim cone */
    float yaw_deviation = actor_def->looking.runtime_maximum_looking_deviation_cosine.n[0];

    /* The look-cone vector overlaps the prop iterator: the iterator's index sits at look_cone.n[2]. */
    struct
    {
        float look_cone[2];
        prop_iterator iterator;
    } look_state;

    if (actor->state.mode == _actor_mode_combat)
    {
        look_state.look_cone[0] = cos(actor_def->looking.combat_looking_delta_angles[0]);
        look_state.look_cone[1] = cos(actor_def->looking.combat_looking_delta_angles[1]);
    }
    else
    {
        look_state.look_cone[0] = cos(actor_def->looking.noncombat_looking_delta_angles[0]);
        look_state.look_cone[1] = cos(actor_def->looking.noncombat_looking_delta_angles[1]);
    }
    real_vector3d *facing_vector = (real_vector3d *)&look_state;

    prop_iterator_new(&look_state.iterator, actor_index);

    int best_prop_index = -1;
    prop_datum *best_prop = nullptr;
    float best_score = 0.0f;
    uint8_t best_newly_interesting = 0;

    prop_datum *prop = prop_iterator_next(&look_state.iterator);
    if (!prop)
        return 0;

    do
    {
        int prop_type = prop->state;
        if (prop_type >= _prop_state_becoming_unacknowledged && prop_type <= _prop_state_acknowledged && prop->visibility)
        {
            float interest = prop->look_interest;
            if (interest > 0.0f)
            {
                int last_look_time = prop->last_idle_look_time;
                float freshness;
                if (last_look_time == -1)
                    freshness = 1.0f;
                else
                    freshness = ((float)game_time - (float)last_look_time) * 0.0016666667f - 1.0f;

                float decay = (interest - prop->last_idle_look_interest) / interest + freshness;
                if (decay > 1.0f)
                    decay = 1.0f;
                float score = interest * decay;
                uint8_t newly_interesting = interest > prop->last_idle_look_interest;

                if (score > 0.0f)
                {
                    uint8_t valid;
                    if (aim_at_prop)
                    {
                        if (free_facing && interest > prop->last_idle_look_interest)
                        {
                            valid = 1;
                        }
                        else
                        {
                            valid = 0;
                            /* binary reads *((_QWORD *)prop + 28) (byte 0xE0) as two floats: the 2D actor->prop direction */
                            float dir_x = prop->actor_to_prop.n[0];
                            float dir_y = prop->actor_to_prop.n[1];
                            float magnitude = __fsqrts(dir_x * dir_x + dir_y * dir_y);
                            if (__fabs(magnitude) >= 0.000099999997f && magnitude > 0.0f)
                            {
                                float normalized_x = (1.0f / magnitude) * dir_x;
                                float normalized_y = (1.0f / magnitude) * dir_y;
                                valid = (actor->control.desired_facing_vector.n[1] * normalized_y
                                         + actor->control.desired_facing_vector.n[0] * normalized_x) > look_cone_threshold;
                            }
                        }
                    }
                    else
                    {
                        /* Args verified via disasm @0x837FB148-0x837FB158:
                         * r4=facing_vector, r5=&control.desired_facing_vector (0x5A4),
                         * r6=&control.desired_aiming_vector (addi r6,r29,0x5B0), r7=prop+0xE0. */
                        valid = actor_look_valid_look_vector(yaw_deviation,
                                                             (const real_vector2d *)facing_vector,
                                                             (const real_vector2d *)&actor->control.desired_facing_vector,
                                                             (const real_vector2d *)&actor->control.desired_aiming_vector,
                                                             (const real_vector2d *)&prop->actor_to_prop);
                    }

                    if (valid && score > best_score)
                    {
                        best_prop_index = look_state.iterator.index;
                        best_prop = prop;
                        best_score = score;
                        best_newly_interesting = newly_interesting;
                    }
                }
            }
        }
        else
        {
            prop->last_idle_look_interest = 0.0f;
        }

        prop = prop_iterator_next(&look_state.iterator);
    }
    while (prop);

    if (best_prop_index == -1)
        return 0;

    best_prop->last_idle_look_interest = best_prop->look_interest;  /* refresh the look timer */
    best_prop->last_idle_look_time = game_time;                     /* record the last-look time */
    direction->type = _direction_specification_prop;
    direction->___u1.prop_index = best_prop_index;
    *interesting_prop = best_newly_interesting;
    return 1;
}
