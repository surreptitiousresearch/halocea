/* equipment_update_baseline @0x837B62B8 — snapshots network-baseline scratch fields for an equipment
 * object: copies 6 consecutive dwords (+92..+112, i.e. indices 23-28) and 3 more (+140..+148, indices
 * 35-37) into a 9-dword baseline block at +584 (indices 146-154), then advances the baseline generation
 * byte (+581), marks +580 dirty, and clears +582.
 *
 * Fields resolved to the DB equipment_datum struct (equipment.baseline position/velocities,
 * baseline_valid/baseline_index/message_index), matching biped_update_baseline.c's generation-byte idiom. */

#include "headers/equipment_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void equipment_update_baseline(int object_index)
{
    equipment_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_equipment);

    if ( object )
    {
        unsigned __int8 next_generation = object->equipment.baseline_index + 1;

        /* snapshot current motion state into the baseline block (indices 146-154 = +584..+616) */
        object->equipment.baseline.position = object->object.position;
        object->equipment.baseline.translational_velocity = object->object.translational_velocity;
        object->equipment.baseline.angular_velocity = object->object.angular_velocity;

        object->equipment.baseline_valid = 1;
        object->equipment.baseline_index = next_generation;
        object->equipment.message_index = 0;
    }
}
