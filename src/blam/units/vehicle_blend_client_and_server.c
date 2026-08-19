/* vehicle_blend_client_and_server @0x8375D168 — network smoothing for a client-side vehicle: reconcile the
 * locally simulated ("client") transform with the last authoritative ("server") transform. When the object's
 * orientation-sync flag is set it blends the stored server orientation toward the current one (half way); if the
 * blend reports no meaningful change it snaps the server orientation to the current orientation. It then decides
 * the render position: if the client and server positions are more than 5 world units apart, or object
 * prediction is disabled, it uses the client position as-is; otherwise it dead-reckons the vehicle, interpolates
 * halfway between the server position and the dead-reckoned position, and uses that unless any component is NaN
 * or outside +/-5000 (in which case it falls back to the client position).
 *
 * DEVIATION: the vehicle_blend_orientations call args and the points_interpolate result pointer were recovered
 * from disasm (FPR-shadow: the 0.5 blend scale burns a GPR slot, so the decompiler mislabeled forward_out/up_out
 * and lost the interpolate destination). forward_out/up_out are object+116/+128; the interpolate result is the
 * local `interpolated`. Register-pun doubles are plain float math. The vehicle object is not modeled; its
 * transform fields are read at raw offsets (orientation forward/up at +44/+56 current, +116/+128 stored;
 * positions at +28 client and +92 server). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include <math.h>
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern float __fsqrts(float value);
extern int _isnan(double x);
extern uint8_t vehicle_blend_orientations(const real_vector3d *forward_1, const real_vector3d *up_1, const real_vector3d *forward_2, const real_vector3d *up_2, float blend_scale, real_vector3d *forward_out, real_vector3d *up_out);
extern uint8_t object_dead_recon_position(int object_index, real_point3d *predicted_position);
extern void points_interpolate(const real_point3d *a, const real_point3d *b, float t, real_point3d *result);
extern void vehicle_set_position_and_correct_children(int vehicle_index, real_point3d *new_position);

void vehicle_blend_client_and_server(int vehicle_index)
{
    vehicle_datum *vehicle_object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    if ( vehicle_object->object.is_server_orientation_valid == 1
      && !vehicle_blend_orientations(
             &vehicle_object->object.forward,
             &vehicle_object->object.up,
             &vehicle_object->object.last_server_forward,
             &vehicle_object->object.last_server_up,
             0.5f,
             &vehicle_object->object.forward,
             &vehicle_object->object.up) )
    {
        vehicle_object->object.forward.n[0] = vehicle_object->object.last_server_forward.n[0];
        vehicle_object->object.forward.n[1] = vehicle_object->object.last_server_forward.n[1];
        vehicle_object->object.forward.n[2] = vehicle_object->object.last_server_forward.n[2];
        vehicle_object->object.up.n[0] = vehicle_object->object.last_server_up.n[0];
        vehicle_object->object.up.n[1] = vehicle_object->object.last_server_up.n[1];
        vehicle_object->object.up.n[2] = vehicle_object->object.last_server_up.n[2];
    }

    real_point3d server_position;
    server_position.n[0] = vehicle_object->object.position.n[0];
    server_position.n[1] = vehicle_object->object.position.n[1];
    server_position.n[2] = vehicle_object->object.position.n[2];

    real_point3d client_position;
    client_position.n[0] = vehicle_object->object.last_server_position.n[0];
    client_position.n[1] = vehicle_object->object.last_server_position.n[1];
    client_position.n[2] = vehicle_object->object.last_server_position.n[2];

    real_point3d interpolated;
    real_point3d *result_position;

    float dy = client_position.n[1] - server_position.n[1];
    float dz = client_position.n[2] - server_position.n[2];
    float dx = client_position.n[0] - server_position.n[0];
    if ( __fsqrts(dy * dy + (dz * dz + dx * dx)) > 5.0f || !object_prediction )
    {
        result_position = &client_position;
    }
    else
    {
        real_point3d dead_recon;
        if ( !object_dead_recon_position(vehicle_index, &dead_recon) )
            dead_recon = client_position;

        points_interpolate(&server_position, &dead_recon, 0.5f, &interpolated);

        if ( _isnan(interpolated.n[0]) || interpolated.n[0] < -5000.0f || interpolated.n[0] > 5000.0f
          || _isnan(interpolated.n[1]) || interpolated.n[1] < -5000.0f || interpolated.n[1] > 5000.0f
          || _isnan(interpolated.n[2]) || interpolated.n[2] < -5000.0f || interpolated.n[2] > 5000.0f )
            result_position = &client_position;
        else
            result_position = &interpolated;
    }

    vehicle_set_position_and_correct_children(vehicle_index, result_position);
}
