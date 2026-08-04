/* vehicle_set_position_and_correct_children @0x8375CF7C — move a vehicle to a new position while keeping
 * its seated children consistent: shift each child object (in seats node 0/1) by the same delta applied to
 * the vehicle, then translate the vehicle itself. The child position offset lives at +844/+848/+852. */

#include "headers/data_array.h"
#include "headers/object_type.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

void vehicle_set_position_and_correct_children(int vehicle_index, real_point3d *new_position)
{
    object_datum *vehicle =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    int child_index = vehicle->object.first_child_object_index;

    float delta_x = new_position->n[0] - vehicle->object.position.n[0];
    float delta_y = new_position->n[1] - vehicle->object.position.n[1];
    float delta_z = new_position->n[2] - vehicle->object.position.n[2];

    unit_datum *child;
    for (; child_index != -1; child_index = child->object.next_object_index)
    {
        child = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                 child_index)->datum;
        if (((1 << child->object.type) & object_mask_unit) != 0)
        {
            child->unit.seat_last_position.n[0] += delta_x;
            child->unit.seat_last_position.n[1] += delta_y;
            child->unit.seat_last_position.n[2] += delta_z;
        }
    }

    gearbox_object_translate(vehicle_index, new_position);
}
