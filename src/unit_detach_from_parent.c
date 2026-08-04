/* unit_detach_from_parent @0x836CAA94 — eject a unit from the object it is riding/attached to. Computes a
 * separation direction (parent origin -> unit origin, falling back to the unit's forward vector when the
 * origins coincide), detaches the object, re-seats it at its current position clamped into the world, adds
 * a small outward velocity impulse, clears the "attached" and parented flags, and recomputes node
 * matrices. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_flags.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern float normalize3d(real_vector3d *v);
extern void object_detach(int child_object_index);
extern int scenario_ensure_point_within_world(real_point3d *point);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void object_compute_node_matrices(int object_index);

void unit_detach_from_parent(int object_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    int parent_index = unit->object.parent_object_index;
    if (parent_index == -1)
        return;

    real_point3d parent_origin;
    real_point3d unit_origin;
    object_get_origin(parent_index, &parent_origin);
    object_get_origin(object_index, &unit_origin);

    real_vector3d separation;   /* a difference of points is a direction vector */
    separation.n[0] = unit_origin.n[0] - parent_origin.n[0];
    separation.n[1] = unit_origin.n[1] - parent_origin.n[1];
    separation.n[2] = unit_origin.n[2] - parent_origin.n[2];
    if (normalize3d(&separation) == 0.0f)
    {
        separation.n[0] = unit->object.forward.i;
        separation.n[1] = unit->object.forward.j;
        separation.n[2] = unit->object.forward.k;
    }

    float impulse_x = (separation.n[0] * 0.020000001f);
    float impulse_y = (separation.n[1] * 0.020000001f);
    float impulse_z = (separation.n[2] * 0.020000001f);

    object_detach(object_index);

    real_point3d position;
    position.n[0] = unit->object.position.n[0];
    position.n[1] = unit->object.position.n[1];
    position.n[2] = unit->object.position.n[2];
    scenario_ensure_point_within_world(&position);
    object_set_position(object_index, &position, nullptr, nullptr);

    int object_flags = unit->object.flags;
    int unit_flags = unit->unit.flags;
    unit->unit.force_local_update = 1;
    unit->object.flags = object_flags & ~(1u << _object_at_rest_bit);
    unit->unit.flags = unit_flags & ~(1u << _unit_attached_melee_attack_bit);
    unit->object.translational_velocity.i += impulse_x;
    unit->object.translational_velocity.j += impulse_y;
    unit->object.translational_velocity.k += impulse_z;

    object_set_visibility(object_index, 1u);
    object_compute_node_matrices(object_index);
}
