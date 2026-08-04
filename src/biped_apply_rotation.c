/* biped_apply_rotation @0x837AE798 — advance a biped's facing by the pending angular delta. The delta is
 * stored on the object as an axis*angle vector (at +140); its magnitude is the rotation angle and its
 * direction the rotation axis. The biped's forward (+116) and up (+128) vectors are rotated about that axis,
 * forward is renormalized, and up is re-orthogonalized against forward via up = (forward x up) x forward.
 * If the result degenerates (zero-length up) the orientation is reset to the world forward/up.
 *
 * DEVIATION: the decompiler invented a trailing `long double a2` parameter; it is actually a scratch double
 * the angle is staged through for cos/sin (the DB prototype takes only biped_index). The int<->float
 * conversion shuffles and __PAIR64__ casts were restored to plain float math. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern double sin(double x);
extern double cos(double x);

void biped_apply_rotation(int biped_index)
{
    biped_datum *biped_object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    real_vector3d *forward = &biped_object->object.forward;
    real_vector3d *up = &biped_object->object.up;

    /* Pending rotation: axis*angle. The vector's length is the angle, its direction the axis. */
    real_vector3d axis;
    axis.n[0] = biped_object->object.angular_velocity.n[0];
    axis.n[1] = biped_object->object.angular_velocity.n[1];
    axis.n[2] = biped_object->object.angular_velocity.n[2];

    float angle = __fsqrts(axis.n[0] * axis.n[0] + axis.n[1] * axis.n[1] + axis.n[2] * axis.n[2]);
    if (__fabs(angle) < 0.000099999997f)
    {
        angle = 0.0f;
    }
    else
    {
        float inverse = 1.0f / angle;
        axis.n[0] *= inverse;
        axis.n[1] *= inverse;
        axis.n[2] *= inverse;
    }

    float sine = (float)sin(angle);
    float cosine = (float)cos(angle);

    rotate_vector_about_axis(forward, &axis, sine, cosine);

    float forward_length = __fsqrts(forward->n[0] * forward->n[0]
                                  + forward->n[1] * forward->n[1]
                                  + forward->n[2] * forward->n[2]);
    if (__fabs(forward_length) >= 0.000099999997f)
    {
        float inverse = 1.0f / forward_length;
        forward->n[0] *= inverse;
        forward->n[1] *= inverse;
        forward->n[2] *= inverse;
    }

    real_vector3d rotated_up;
    rotated_up.n[0] = up->n[0];
    rotated_up.n[1] = up->n[1];
    rotated_up.n[2] = up->n[2];
    rotate_vector_about_axis(&rotated_up, &axis, sine, cosine);

    float fx = forward->n[0], fy = forward->n[1], fz = forward->n[2];
    float cross_x = fy * rotated_up.n[2] - fz * rotated_up.n[1];
    float cross_y = fz * rotated_up.n[0] - fx * rotated_up.n[2];
    float cross_z = fx * rotated_up.n[1] - fy * rotated_up.n[0];

    up->n[0] = fz * cross_y - fy * cross_z;
    up->n[1] = fx * cross_z - fz * cross_x;
    up->n[2] = fy * cross_x - fx * cross_y;

    float up_length = __fsqrts(up->n[0] * up->n[0] + up->n[1] * up->n[1] + up->n[2] * up->n[2]);
    if (__fabs(up_length) < 0.000099999997f
        || (up->n[0] *= 1.0f / up_length,
            up->n[1] *= 1.0f / up_length,
            up->n[2] *= 1.0f / up_length,
            up_length == 0.0f))
    {
        *forward = *global_forward3d;
        *up = *global_up3d;
    }
}
