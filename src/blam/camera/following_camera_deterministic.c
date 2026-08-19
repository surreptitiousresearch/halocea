/* following_camera_deterministic @0x837C2978 — third-person "following" camera placement: start from the
 * unit's own camera position/forward, then offset it by camera_track_splut's tracking vector (rotated by
 * the unit's yaw — normalized forward.x/y — around the vertical axis; z is added directly).
 *
 * DEVIATION: the decompiler renders camera_track_splut's 3rd argument as an unused/uninitialized local;
 * disasm confirms it's the DB's real `real_vector3d *offset` out-parameter (a stack local the callee
 * writes into), matching the DB's 3-arg prototype. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_camera.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern unit_camera * unit_camera_get(int unit_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern double asin(double x);
extern float __fsqrts(float x);
extern double __fabs(double x);
extern void camera_track_splut(const unit_camera *camera, float pitch, real_vector3d *offset);

void following_camera_deterministic(int unit_index, real_point3d *position, real_vector3d *forward)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    const unit_camera *camera = unit_camera_get(unit_index);

    unit_get_camera_position(unit_index, position);
    *forward = unit->unit.aiming_vector;   /* unit+572 = _unit_datum.aiming_vector */

    float pitch = (float)asin(forward->n[2]);

    real_vector3d offset;
    camera_track_splut(camera, pitch, &offset);

    float forward_x = forward->n[0];
    float forward_y = forward->n[1];
    float horizontal_magnitude = __fsqrts(forward->n[1] * forward->n[1] + forward->n[0] * forward->n[0]);
    if ( __fabs(horizontal_magnitude) >= 0.0001f )
    {
        float inverse_magnitude = 1.0f / horizontal_magnitude;
        forward_x = inverse_magnitude * forward->n[0];
        forward_y = inverse_magnitude * forward->n[1];
    }

    float original_x = position->n[0];
    float original_y = position->n[1];

    position->n[2] = position->n[2] + offset.n[2];
    position->n[0] = offset.n[1] * forward_y + offset.n[0] * forward_x + original_x;
    position->n[1] = offset.n[0] * forward_y - offset.n[1] * forward_x + original_y;
}
