/* editor_camera_flying_update @0x837C3F50 — per-frame update for the free-flying editor/debug camera. When
 * input is active it accumulates the look deltas into the camera's euler orientation (clamping pitch to
 * +/-1.5676548 rad ~ 89.8 deg) and either accumulates or zeroes roll depending on the global use_roll toggle.
 * It then produces the observer command: forward from the orientation, up derived from forward (the horizontal
 * projection normalized, cross-multiplied into an up vector, then rolled about forward by the camera roll), and
 * a position advanced by the input position-deltas rotated into the camera's yaw frame and scaled by the global
 * editor `speed`. When a unit is focused (unit_focus valid) the movement instead accumulates into unit_offset
 * and the eye rides at the focused object's origin plus that offset. The command's focus tracks the eye, focus
 * offset/distance are zeroed, and the FOV is fixed at 1.2217305 rad (70 deg).
 *
 * DEVIATION: register-pun long doubles feeding cos/sin are plain double soft-float calls; the pitch sin() whose
 * result is discarded is kept (the compiler emitted it) but drops out. v18 (=1/mag * 0.0) is always 0; retained
 * verbatim. The object datum origin is read at float offset 40/41/42 (=+160) per the live-datum convention. */

#include "headers/flying_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include <math.h>
#include "headers/object_type.h"
#include "headers/observer_command_flags.h"
#include "headers/blam_data_globals.h"


extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern double cos(double x);
extern double sin(double x);
extern float __fsqrts(float x);
extern float __fabs(float x);

void editor_camera_flying_update(flying_camera *camera, const camera_control *controls, observer_command *result)
{
    if ( controls->active )
    {
        camera->orientation.n[0] = controls->facing_delta.n[0] + camera->orientation.n[0];
        float pitch = controls->facing_delta.n[1] + camera->orientation.n[1];
        if ( pitch < -1.5676548f )
            pitch = -1.5676548f;
        else if ( pitch > 1.5676548f )
            pitch = 1.5676548f;
        camera->orientation.n[1] = pitch;
        if ( use_roll )
            camera->roll = controls->facing_delta.n[2] + camera->roll;
        else
            camera->roll = 0.0f;
    }

    result->timer = 0.30000001f;
    vector3d_from_euler_angles2d(&result->___u1.__s0.forward, &camera->orientation);

    float negated_x = -result->___u1.__s0.forward.n[0];
    float horizontal_magnitude =
            __fsqrts(((negated_x * negated_x) + (result->___u1.__s0.forward.n[1] * result->___u1.__s0.forward.n[1])));

    float basis_x;   /* v17 */
    float basis_y;   /* v18 (always 0) */
    float basis_z;   /* v16 */
    if ( __fabs(horizontal_magnitude) < 0.000099999997f || horizontal_magnitude == 0.0f )
    {
        basis_z = 1.0f;
        basis_y = 0.0f;
        basis_x = 0.0f;
    }
    else
    {
        float inverse = 1.0f / horizontal_magnitude;
        basis_x = inverse * negated_x;
        basis_y = inverse * 0.0f;
        basis_z = inverse * result->___u1.__s0.forward.n[1];
    }

    result->___u1.__s0.up.n[0] = (result->___u1.__s0.forward.n[2] * basis_x) - (result->___u1.__s0.forward.n[1] * basis_y);
    result->___u1.__s0.up.n[2] = (result->___u1.__s0.forward.n[1] * basis_z) - (result->___u1.__s0.forward.n[0] * basis_x);
    result->___u1.__s0.up.n[1] = (result->___u1.__s0.forward.n[0] * basis_y) - (result->___u1.__s0.forward.n[2] * basis_z);

    rotate_vector_about_axis(&result->___u1.__s0.up, &result->___u1.__s0.forward, (float)sin(camera->roll), (float)cos(camera->roll));

    float yaw_cos = (float)cos(camera->orientation.n[0]);
    float yaw_sin = (float)sin(camera->orientation.n[0]);
    cos(camera->orientation.n[1]);
    sin(camera->orientation.n[1]);   /* result discarded — emitted by the compiler */

    float local_x = ((controls->position_delta.n[0] * yaw_cos)
            - (controls->position_delta.n[1] * yaw_sin));
    float move_y = (speed * ((controls->position_delta.n[1] * yaw_cos)
            + (controls->position_delta.n[0] * yaw_sin)));
    float move_z = (speed * controls->position_delta.n[2]);
    float move_x = (speed * local_x);

    float new_x;
    float new_y;
    float new_z;
    if ( unit_focus == -1 || !object_try_and_get_and_verify_type(unit_focus, object_mask_all) )
    {
        new_x = camera->position.n[0] + move_x;
        new_y = camera->position.n[1] + move_y;
        new_z = camera->position.n[2] + move_z;
    }
    else
    {
        unit_offset.n[0] = unit_offset.n[0] + move_x;
        unit_offset.n[1] = unit_offset.n[1] + move_y;
        unit_offset.n[2] = unit_offset.n[2] + move_z;
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_focus)->datum;
        new_x = object->object.bounding_sphere_center.n[0] + unit_offset.n[0];
        new_y = object->object.bounding_sphere_center.n[1] + unit_offset.n[1];
        new_z = object->object.bounding_sphere_center.n[2] + unit_offset.n[2];
    }

    camera->position.n[0] = new_x;
    camera->position.n[1] = new_y;
    camera->position.n[2] = new_z;
    result->___u1.__s0.focus_position.n[0] = new_x;
    result->___u1.__s0.focus_position.n[1] = new_y;
    result->___u1.__s0.focus_position.n[2] = new_z;
    result->___u1.__s0.focus_offset = *global_zero_vector3d;
    result->___u1.__s0.focus_distance = 0.0f;
    result->___u1.__s0.field_of_view = 1.2217305f;
    result->flags = (1u << _observer_command_valid_bit);
}
