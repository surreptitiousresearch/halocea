/* projectile_handle_parent_destroyed @0x8375A538 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_datum_flags.h"
#include "headers/blam_data_globals.h"

extern void object_detach(int child_object_index);

uint8_t projectile_handle_parent_destroyed(int projectile_index)
{
    projectile_datum *object =
        (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    unsigned int flags = object->projectile.flags;

    object->projectile.arming_time = 1.0f;
    object->projectile.detonation_timer = 1.0f;
    object->projectile.flags = flags & ~(1u << _projectile_attached_bit);
    object_detach(projectile_index);
    return 1;
}
