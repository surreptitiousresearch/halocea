/* unit_get_center_of_mass @0x836C9490 — return the world position of the unit's "body" model marker. */

#include <stdint.h>
#include "headers/object_marker.h"
#include "headers/real_point3d.h"

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void unit_get_center_of_mass(int unit_index, real_point3d *center_of_mass)
{
    object_marker marker;
    object_get_marker_by_name(unit_index, "body", &marker, 1);
    center_of_mass->n[0] = marker.matrix.n[3][0];
    center_of_mass->n[1] = marker.matrix.n[3][1];
    center_of_mass->n[2] = marker.matrix.n[3][2];
}
