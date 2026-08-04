/* unit_get_head_position @0x836C90D0 — return the world position of the unit's "head" model marker. */

#include <stdint.h>
#include "headers/object_marker.h"
#include "headers/real_point3d.h"

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void unit_get_head_position(int unit_index, real_point3d *head_position)
{
    object_marker marker;
    object_get_marker_by_name(unit_index, "head", &marker, 1);
    head_position->n[0] = marker.matrix.n[3][0];
    head_position->n[1] = marker.matrix.n[3][1];
    head_position->n[2] = marker.matrix.n[3][2];
}
