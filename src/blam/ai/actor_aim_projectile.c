/* actor_aim_projectile @0x837B89E0 — compute the firing direction for an actor's projectile shot from `origin`,
 * clamped to within 30 degrees of the actor's weapon vector, and report the aiming error. Returns the target's
 * unit index when the actor is firing at a prop whose perception state is in the acknowledged band
 * (_prop_state_becoming_unacknowledged .. _prop_state_acknowledged, 2..3; `cmpwi r10,2/blt @0x837B8A60`,
 * `cmpwi r10,3/bgt @0x837B8A68`), otherwise -1. The orphan rungs of prop_perception_state are 4/5 and are
 * excluded by this test. When the actor has a
 * cached aim vector (actor->control.burst_aim_by_vector) it is used directly; otherwise the direction to the
 * actor's aim point (actor->control.burst_target) is used. If the desired direction lies more than 30 degrees
 * from the weapon vector, it is snapped onto the 30-degree cone around the weapon vector (rotating about the
 * perpendicular axis; if the two are collinear an arbitrary perpendicular is used).
 *
 * Named actor fields used: control.fire_state (==2 gate), control.current_fire_target_type,
 * control.burst_target, control.burst_aim_by_vector, control.burst_aim_vector, control.burst_error.
 * 0.86602539 = cos(30 deg), 0.5 = sin(30 deg). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_fire_state.h"
#include "headers/actor_fire_target_type.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void actor_get_weapon_vector(int actor_index, real_vector3d *weapon_vector);
extern float normalize3d(real_vector3d *v);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

int actor_aim_projectile(int actor_index, const real_point3d *origin, real_vector3d *vector, float *error_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int target_unit_index = -1;

    if ( actor->control.fire_state != actor_fire_state_bursting )
        return target_unit_index;

    /* DEVIATION: decompiler punned the prop-index read through &current_fire_target_type; disasm loads the full
     * word at +0x610 = ___u58.current_fire_target_prop_index (union at 0x1A4, after the int16 type at 0x1A0). */
    if ( actor->control.current_fire_target_type == actor_fire_target_prop && actor->control.___u58.current_fire_target_prop_index != -1 )
    {
        prop_datum *prop = DATUM_GET(prop_data, prop_datum, actor->control.___u58.current_fire_target_prop_index);
        if ( prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged )
            target_unit_index = prop->unit_index;
    }

    if ( actor->control.burst_aim_by_vector )
    {
        *vector = actor->control.burst_aim_vector;
    }
    else
    {
        vector->n[0] = actor->control.burst_target.n[0] - origin->n[0];
        vector->n[1] = actor->control.burst_target.n[1] - origin->n[1];
        vector->n[2] = actor->control.burst_target.n[2] - origin->n[2];
        normalize3d(vector);
    }

    real_vector3d weapon_vector;
    actor_get_weapon_vector(actor_index, &weapon_vector);
    float alignment = vector->n[2] * weapon_vector.n[2]
        + (vector->n[0] * weapon_vector.n[0] + vector->n[1] * weapon_vector.n[1]);
    if ( alignment < 0.86602539f )
    {
        real_vector3d rotation_axis;
        char have_axis = 1;
        rotation_axis.n[0] = vector->n[2] * weapon_vector.n[1] - vector->n[1] * weapon_vector.n[2];
        rotation_axis.n[1] = vector->n[0] * weapon_vector.n[2] - vector->n[2] * weapon_vector.n[0];
        rotation_axis.n[2] = vector->n[1] * weapon_vector.n[0] - vector->n[0] * weapon_vector.n[1];
        if ( normalize3d(&rotation_axis) == 0.0f )
        {
            perpendicular3d(&weapon_vector, &rotation_axis);
            have_axis = normalize3d(&rotation_axis) != 0.0f;
        }
        *vector = weapon_vector;
        if ( have_axis )
            rotate_vector_about_axis(vector, &rotation_axis, 0.5f, 0.86602539f);
    }

    *error_reference = actor->control.burst_error;
    return target_unit_index;
}
