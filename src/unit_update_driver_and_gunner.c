/* unit_update_driver_and_gunner @0x836CAED8 — for a vehicle, scans its child units and records which one
 * occupies the driver seat (unit.driver_object_index) and which occupies the gunner seat
 * (unit.gunner_object_index). The driver is only assigned when the vehicle is not flagged 0x1 and has no
 * driver yet; the gunner is assigned when the gunner seat is free or currently held by the driver. Seat
 * roles come from the seat block in the vehicle definition (driver bit 0x4, gunner bit 0x8). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_flags.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


void unit_update_driver_and_gunner(int parent_object_index)
{
    unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum);
    unit_definition *definition = TAG_GET(unit_definition, parent->definition_index);

    for (int child = parent->object.first_child_object_index; child != -1; )
    {
        unit_datum *child_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
        int next = child_object->object.next_object_index;

        if (((1 << (unsigned __int16)child_object->object.type) & object_mask_unit) == 0)  /* biped or vehicle only */
        {
            child = next;
            continue;
        }
        int seat_index = child_object->unit.parent_seat_index;
        if (seat_index == -1)
        {
            child = next;
            continue;
        }

        unit_seat *seats_block = (unit_seat *)definition->unit.seats.address;
        unsigned int seat_flags = seats_block[seat_index].flags;

        int assign_gunner;
        if ((seat_flags & (1u << _unit_seat_is_driver_bit)) != 0 && (parent->unit.flags & (1u << _unit_actively_controlled_bit)) == 0 && parent->unit.driver_object_index == -1)
        {
            parent->unit.driver_object_index = child;
            if ((seat_flags & (1u << _unit_seat_is_gunner_bit)) == 0)
            {
                child = next;
                continue;
            }
            assign_gunner = (parent->unit.gunner_object_index == -1);
        }
        else
        {
            if ((seat_flags & (1u << _unit_seat_is_gunner_bit)) == 0)
            {
                child = next;
                continue;
            }
            int gunner = parent->unit.gunner_object_index;
            if (gunner == -1)
            {
                parent->unit.gunner_object_index = child;
                child = next;
                continue;
            }
            assign_gunner = (gunner == parent->unit.driver_object_index);
        }

        if (assign_gunner)
            parent->unit.gunner_object_index = child;
        child = next;
    }
}
