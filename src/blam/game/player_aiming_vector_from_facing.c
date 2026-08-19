/* player_aiming_vector_from_facing @0x836A7E68 — convert a player's desired 2D facing into a world-space
 * aiming vector. Normally the aim is just the facing direction. When the player is riding a vehicle whose
 * occupied seat is not flagged "aim in world space" (seat flag 0x10 clear), the aim is reoriented into the
 * vehicle's frame: a side vector is built as up x right (falling back to up x down when degenerate), a
 * rotation matrix is formed from that side and the vehicle up axis, and the facing-derived aim is
 * transformed by it. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern float normalize3d(real_vector3d *v);
extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);

void player_aiming_vector_from_facing(int player_index, real_vector3d *aiming_vector,
                                      const real_euler_angles2d *desired_facing)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    vector3d_from_euler_angles2d(aiming_vector, desired_facing);

    if (player->unit_index == -1)
        return;

    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;
    int parent_index = unit->object.parent_object_index;
    if (parent_index == -1)
        return;

    unit_datum *vehicle = object_try_and_get_and_verify_type(parent_index, object_mask_vehicle);
    if (!vehicle)
        return;

    unit_definition *vehicle_definition = TAG_GET(unit_definition, vehicle->definition_index);
    unit_seat *seat = (unit_seat *)vehicle_definition->unit.seats.address + unit->unit.parent_seat_index;
    if ((seat->flags & (1u << _unit_seat_has_third_person_camera_bit)) != 0)
        return;

    /* DEVIATION from prior draft: absolute offset 128 (0x80) is object.up, not forward (0x74). */
    const real_vector3d *vehicle_up = &vehicle->object.up;
    real_matrix4x3 orientation;

    /* side = vehicle_up x global_right3d */
    orientation.n[0][2] = (vehicle_up->n[0] * global_right3d->n[1]) - (global_right3d->n[0] * vehicle_up->n[1]);
    orientation.n[0][1] = (vehicle_up->n[2] * global_right3d->n[0]) - (global_right3d->n[2] * vehicle_up->n[0]);
    orientation.n[0][0] = (global_right3d->n[2] * vehicle_up->n[1]) - (vehicle_up->n[2] * global_right3d->n[1]);

    if (normalize3d((real_vector3d *)&orientation.n[0]) == 0.0f)
    {
        /* up parallel to right: fall back to up x global_down3d */
        orientation.n[0][2] = (global_down3d->n[1] * vehicle_up->n[0]) - (global_down3d->n[0] * vehicle_up->n[1]);
        orientation.n[0][1] = (vehicle_up->n[2] * global_down3d->n[0]) - (global_down3d->n[2] * vehicle_up->n[0]);
        orientation.n[0][0] = (global_down3d->n[2] * vehicle_up->n[1]) - (vehicle_up->n[2] * global_down3d->n[1]);
        normalize3d((real_vector3d *)&orientation.n[0]);
    }

    matrix4x3_rotation_from_vectors(&orientation, (const real_vector3d *)&orientation.n[0], vehicle_up);
    matrix4x3_transform_normal(&orientation, aiming_vector, aiming_vector);
}
