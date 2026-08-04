/* unit_can_see_point @ 0x836CA788 — test whether a point lies within the unit's view cone of half-angle
 * theta. Takes the direction from the unit's head marker to the point, normalizes it, dots it with the
 * unit's facing vector (unit floats[152..154] = unit+608/612/616) and returns 1 when that cosine exceeds
 * cos(theta), i.e. the point is inside the cone. Returns 0 for an invalid unit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_marker.h"
#include "headers/blam_data_globals.h"
extern double cos(double x);


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern float normalize3d(real_vector3d *v);

uint8_t unit_can_see_point(int unit_index, const real_point3d *point, float theta)
{
    if ( unit_index == -1 )
        return 0;

    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    object_marker head;
    object_get_marker_by_name(unit_index, "head", &head, 1);

    real_vector3d direction;
    direction.n[0] = point->n[0] - head.matrix.n[3][0];
    direction.n[1] = point->n[1] - head.matrix.n[3][1];
    direction.n[2] = point->n[2] - head.matrix.n[3][2];
    normalize3d(&direction);

    float cosine = ((direction.n[0] * unit->unit.looking_vector.n[0])   /* +608 */
        + ((unit->unit.looking_vector.n[2] * direction.n[2]) + (unit->unit.looking_vector.n[1] * direction.n[1])));
    if ( cosine <= (float)cos(theta) )
        return 0;
    return 1;
}
