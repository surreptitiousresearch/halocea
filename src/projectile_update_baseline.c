/* projectile_update_baseline @0x8375A930 — snapshot a projectile object's live position/velocity
 * (object.position, object.translational_velocity) into its network baseline scratch fields
 * (projectile.baseline.position, projectile.baseline.translational_velocity — the same fields
 * projectile_new_to_network.c reads back out), bump the baseline index (wrapping byte), and mark the
 * baseline valid with its age reset (message_index). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/projectile_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void projectile_update_baseline(int object_index)
{
    projectile_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_projectile);

    if ( object )
    {
        real_point3d position = object->object.position;
        real_vector3d translational_velocity = object->object.translational_velocity;
        uint8_t next_baseline_index = object->projectile.baseline_index + 1;

        object->projectile.baseline_valid = 1;
        object->projectile.baseline_index = next_baseline_index;
        object->projectile.message_index = 0;
        object->projectile.baseline.position = position;
        object->projectile.baseline.translational_velocity = translational_velocity;
    }
}
