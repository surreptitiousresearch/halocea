/* unit_adjust_projectile_ray @0x836CE118 — adjust a projected weapon-fire ray so it visually originates
 * from the camera: optionally snap `forward` to the unit's melee-detonation direction marker (object+572,
 * when `offset_aim`), and optionally re-project `origin` onto the camera-to-original-origin offset along
 * `forward` (when `offset_origin`, moving it to the point on the ray through the camera position closest to
 * the original origin). Always outputs the unit's inherited velocity along `forward` into
 * `*inherited_velocity`. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);

void unit_adjust_projectile_ray(int unit_index, real_point3d *origin, real_vector3d *forward, float *inherited_velocity, uint8_t offset_origin, uint8_t offset_aim)
{
    if ( offset_aim )
    {
        unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        /* +572 = unit_datum.unit.aiming_vector (500 + 0x48) */
        *forward = unit->unit.aiming_vector;
    }

    if ( offset_origin )
    {
        real_point3d camera_position;
        unit_get_camera_position(unit_index, &camera_position);

        float projection = forward->n[0] * (origin->n[0] - camera_position.n[0])
                          + forward->n[1] * (origin->n[1] - camera_position.n[1])
                          + forward->n[2] * (origin->n[2] - camera_position.n[2]);

        origin->n[0] = forward->n[0] * projection + camera_position.n[0];
        origin->n[1] = forward->n[1] * projection + camera_position.n[1];
        origin->n[2] = forward->n[2] * projection + camera_position.n[2];
    }

    real_vector3d velocity;
    object_get_velocities(unit_index, &velocity, nullptr);
    *inherited_velocity = forward->n[0] * velocity.n[0] + forward->n[1] * velocity.n[1] + forward->n[2] * velocity.n[2];
}
