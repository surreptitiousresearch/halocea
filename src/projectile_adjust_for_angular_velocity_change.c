/* projectile_adjust_for_angular_velocity_change @0x83759C10 — recompute a projectile's cached spin axis and
 * per-tick rotation sin/cos from its current angular velocity vector. When the angular velocity is zero,
 * the spin flag (projectile.flags bit 0) is cleared and the rotation is set to identity (sin 0, cos 1). Otherwise the axis is normalized and the rotation angle is the angular speed.
 * DEVIATION: the decompiler shows a phantom 2nd param (a2); it is overwritten with the computed magnitude
 * before any read, so the function is single-argument. Soft-float doubles reproduced as float math. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_datum_flags.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern double sin(double x);
extern double cos(double x);

void projectile_adjust_for_angular_velocity_change(int object_index)
{
    projectile_datum *object = (projectile_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    float wx = object->object.angular_velocity.n[0];
    float wy = object->object.angular_velocity.n[1];
    float wz = object->object.angular_velocity.n[2];
    float magnitude = __fsqrts(wz * wz + (wx * wx + wy * wy));

    if ( magnitude == 0.0f )
    {
        int flags = object->projectile.flags;
        object->projectile.rotation_sine = 0.0f;
        object->projectile.flags = flags & ~(1u << _projectile_has_nonzero_angular_velocity_bit);
        object->projectile.rotation_cosine = 1.0f;
    }
    else
    {
        object->projectile.flags |= (1u << _projectile_has_nonzero_angular_velocity_bit);
        object->projectile.rotation_axis.n[0] = wx * (1.0f / magnitude);
        object->projectile.rotation_axis.n[1] = wy * (1.0f / magnitude);
        object->projectile.rotation_axis.n[2] = wz * (1.0f / magnitude);
        object->projectile.rotation_sine = (float)sin(magnitude);
        object->projectile.rotation_cosine = (float)cos(magnitude);
    }
}
