/* slowly_stop_vehicle @0x8375F668 — ease a vehicle to a halt over a countdown. Each tick decrements the
 * stop countdown (object +1230) and damps both linear (+104) and angular (+140) velocity to 83.5%. The
 * position is advanced by the damped linear velocity, and the orientation (forward +116, up +128) is
 * rotated by the residual angular velocity (treated as an axis-angle, the angle being the spin magnitude).
 * When the countdown reaches zero both velocities are zeroed. The new position/orientation is committed via
 * object_set_position.
 *
 * DEVIATION: the decompiler reads the angular velocity x/y as a packed qword and folds the axis
 * normalization plus a dead `angle == 0` recheck into the branch condition via a comma expression. Since
 * the rotation branch is only taken when |angle| >= 1e-4 (so angle != 0), the normalization is hoisted into
 * that branch and the dead recheck dropped. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern double sin(double x);
extern double cos(double x);
extern void matrix4x3_rotation_from_axis_and_angle(real_matrix4x3 *matrix, const real_vector3d *axis, float sine, float cosine);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);

void slowly_stop_vehicle(int vehicle_index)
{
    vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);

    --vehicle->vehicle.stop_time; /* stop countdown */

    /* damp linear and angular velocity toward zero */
    vehicle->object.translational_velocity.n[0] *= 0.83499998f;
    vehicle->object.translational_velocity.n[1] *= 0.83499998f;
    vehicle->object.translational_velocity.n[2] *= 0.83499998f;
    vehicle->object.angular_velocity.n[0] *= 0.83499998f;
    vehicle->object.angular_velocity.n[1] *= 0.83499998f;
    vehicle->object.angular_velocity.n[2] *= 0.83499998f;

    /* advance the position by the damped linear velocity */
    real_point3d position;
    position.n[0] = vehicle->object.translational_velocity.n[0] + vehicle->object.position.n[0];
    position.n[1] = vehicle->object.position.n[1] + vehicle->object.translational_velocity.n[1];
    position.n[2] = vehicle->object.position.n[2] + vehicle->object.translational_velocity.n[2];

    /* residual angular velocity as an axis-angle (angle = spin magnitude) */
    real_vector3d axis;
    axis.n[0] = vehicle->object.angular_velocity.n[0];
    axis.n[1] = vehicle->object.angular_velocity.n[1];
    axis.n[2] = vehicle->object.angular_velocity.n[2];
    float angle = __fsqrts(axis.n[0] * axis.n[0] + (axis.n[1] * axis.n[1] + axis.n[2] * axis.n[2]));

    real_vector3d forward;
    real_vector3d up;
    if (__fabs(angle) < 0.000099999997f) /* negligible spin: keep current orientation */
    {
        forward = vehicle->object.forward;
        up = vehicle->object.up;
    }
    else
    {
        float inverse = 1.0f / angle;
        axis.n[0] *= inverse;
        axis.n[1] *= inverse;
        axis.n[2] *= inverse;

        real_matrix4x3 rotation;
        matrix4x3_rotation_from_axis_and_angle(&rotation, &axis, (float)sin(angle), (float)cos(angle));
        matrix4x3_transform_vector(&rotation, &vehicle->object.forward, &forward);
        matrix4x3_transform_vector(&rotation, &vehicle->object.up, &up);
    }

    if (!vehicle->vehicle.stop_time) /* countdown expired: full stop */
    {
        vehicle->object.translational_velocity = *global_zero_vector3d;
        vehicle->object.angular_velocity = *global_zero_vector3d;
    }

    object_set_position(vehicle_index, &position, &forward, &up);
}
