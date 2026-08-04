/* antenna_update_attachment @0x8380D5C8 — re-anchor one antenna (springy aerial) to its attachment marker on
 * the parent object. Resolves the marker named by the definition, copies its object-space position into
 * attachment_point and its forward axis into attachment_vector, and derives a scenario location for it. If
 * the attach point has jumped by more than one whole world unit on any axis since last update (e.g. the
 * object teleported), rigidly translates every simulated vertex by the same delta so the spring chain
 * follows instead of snapping taut. Finally records the new attach point as last_attachment_location.
 *
 * The antenna's per-vertex simulation state is the antenna_datum.vertices[] array (antenna_vertex_datum,
 * 32-byte stride) whose current position is member p — the same layout antenna_render_proper.c uses.
 * There are vertices.count + 1 vertices (one more than segments).
 *
 * DEVIATION: the decompiler renders the per-axis "big jump" test as abs32((int)delta) > 1.0. Disasm
 * (0x8380D660-0x8380D6F8) confirms this is literally fctiwz (truncate the float delta toward zero to a
 * 32-bit int), an inlined integer absolute value, then convert back to float and compare > 1.0 — NOT fabsf.
 * So a fractional move like 1.5 truncates to 1 and does not trigger; only a whole-unit move of >= 2 on some
 * axis does. Reproduced faithfully with explicit integer truncation and abs. */

#include <stdint.h>
#include "headers/antenna_datum.h"
#include "headers/antenna_definition.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_marker.h"

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void scenario_location_from_point(location *location, const real_point3d *point);

void antenna_update_attachment(antenna_datum *antenna, antenna_definition *antenna_definition,
    location *attachment_location, real_point3d *attachment_point, real_vector3d *attachment_vector)
{
    object_marker marker;
    object_get_marker_by_name(antenna->object_index, antenna_definition->attachment_marker, &marker, 1);

    *attachment_point = marker.matrix.position;
    *attachment_vector = marker.matrix.forward;
    scenario_location_from_point(attachment_location, &marker.matrix.position);

    float delta_x = attachment_point->n[0] - antenna->last_attachment_location.n[0];
    float delta_y = attachment_point->n[1] - antenna->last_attachment_location.n[1];
    float delta_z = attachment_point->n[2] - antenna->last_attachment_location.n[2];

    int moved_x = (int)delta_x;
    int moved_y = (int)delta_y;
    int moved_z = (int)delta_z;
    if ( moved_x < 0 ) moved_x = -moved_x;
    if ( moved_y < 0 ) moved_y = -moved_y;
    if ( moved_z < 0 ) moved_z = -moved_z;

    if ( (float)moved_x > 1.0f || (float)moved_y > 1.0f || (float)moved_z > 1.0f )
    {
        for ( __int16 i = 0; i < antenna_definition->vertices.count + 1; i = (__int16)(i + 1) )
        {
            /* recovered: (float*)((char*)antenna + 32*i + 28) -> &antenna->vertices[i].p */
            real_point3d *vertex_position = &antenna->vertices[i].p;
            vertex_position->n[0] = vertex_position->n[0] + delta_x;
            vertex_position->n[1] = vertex_position->n[1] + delta_y;
            vertex_position->n[2] = vertex_position->n[2] + delta_z;
        }
    }

    antenna->last_attachment_location.n[0] = attachment_point->n[0];
    antenna->last_attachment_location.n[1] = attachment_point->n[1];
    antenna->last_attachment_location.n[2] = attachment_point->n[2];
}
