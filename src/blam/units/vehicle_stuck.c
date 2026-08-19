/* vehicle_stuck @0x83760E18 — if a vehicle currently has mass points in contact ("stuck"), report
 * the world-space direction from the vehicle origin toward the centroid of those contacting mass
 * points. The per-mass-point contact bitfield lives in the object datum at +1312; only set bits are
 * accumulated. The local centroid is transformed into world space and the offset from the object
 * origin is normalized into approximate_direction. Returns 1 if a meaningful direction was produced,
 * 0 if there is no contact or the offset is degenerate. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"


extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

uint8_t vehicle_stuck(int vehicle_index, real_vector3d *approximate_direction)
{
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    /* was raw VEHICLE_1312 placeholder: per-mass-point contact bitfield at vehicle +1312 */
    int contact_flags = vehicle->vehicle.stuck_mass_point_flags;
    physics_instance instance;
    if (!contact_flags || !physics_instance_new(&instance, vehicle_index))
        return 0;

    int16_t contact_count = 0;
    real_point3d centroid;
    centroid.n[0] = global_origin3d->n[0];
    centroid.n[1] = global_origin3d->n[1];
    centroid.n[2] = global_origin3d->n[2];

    for (int i = 0; i < instance.physics->mass_points.count; i = (int16_t)(i + 1))
    {
        if (((1 << i) & contact_flags) != 0)
        {
            mass_point_definition *mass_point =
                &((mass_point_definition *)instance.physics->mass_points.address)[i];
            ++contact_count;
            centroid.n[0] += mass_point->position.n[0];
            centroid.n[1] += mass_point->position.n[1];
            centroid.n[2] += mass_point->position.n[2];
        }
    }

    if (contact_count <= 0)
        return 0;

    float inverse_count = 1.0f / (float)contact_count;
    centroid.n[0] *= inverse_count;
    centroid.n[1] *= inverse_count;
    centroid.n[2] *= inverse_count;

    real_point3d world_centroid;
    matrix4x3_transform_point(&instance.world_matrix, &centroid, &world_centroid);

    real_point3d origin;
    object_get_origin(vehicle_index, &origin);

    float dx = world_centroid.n[0] - origin.n[0];
    float dy = world_centroid.n[1] - origin.n[1];
    float dz = world_centroid.n[2] - origin.n[2];
    approximate_direction->n[0] = dx;
    approximate_direction->n[1] = dy;
    approximate_direction->n[2] = dz;

    float length = __fsqrts(dx * dx + (dz * dz + dy * dy));
    if (__fabs(length) < 0.000099999997f)
        return 0;

    float inverse_length = 1.0f / length;
    approximate_direction->n[0] = dx * inverse_length;
    approximate_direction->n[1] = dy * inverse_length;
    approximate_direction->n[2] = dz * inverse_length;
    if (length == 0.0f)
        return 0;
    return 1;
}
