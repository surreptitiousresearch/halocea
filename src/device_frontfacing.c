/* device_frontfacing @0x837B5248 — returns true unless the supplied direction vector points into the
 * back face of the device's "front" marker. Used to decide whether a player is approaching a device
 * (e.g. a door) from the operable side. Devices with no "front" marker, or that are usable from
 * both sides (_control_usable_from_both_sides_bit of control_datum control.flags @+0x214), are
 * always treated as front-facing. */

#include <stdint.h>
#include "headers/control_datum.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/control_datum_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

uint8_t device_frontfacing(int device_index, const real_point3d *point, const real_vector3d *vector)
{
    control_datum *device = object_try_and_get_and_verify_type(device_index, object_mask_control);
    if (!device)
        return 1;
    if ((device->control.flags & (1u << _control_usable_from_both_sides_bit)) != 0)
        return 1;

    object_marker front_marker;
    if (object_get_marker_by_name(device_index, "front", &front_marker, 1) != 1)
        return 1;

    float facing = vector->n[0] * front_marker.matrix.n[0][0]
                 + vector->n[1] * front_marker.matrix.n[0][1]
                 + vector->n[2] * front_marker.matrix.n[0][2];
    if (facing <= 0.0f)
        return 1;
    return 0;
}
