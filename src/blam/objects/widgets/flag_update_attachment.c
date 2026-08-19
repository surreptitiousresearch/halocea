/* flag_update_attachment @0x8380DFC8 — re-anchor a CTF flag's cloth simulation to its attachment markers on
 * the flag object. For each attachment point in the flag definition it resolves the named marker and copies
 * the marker's object-space position into attachment_points[i], then derives a scenario location for the
 * first one. If the flag has begun simulating (flag->__noop clear) it:
 *   1. builds the row→attachment-point maps (attachment_y / y_attachments) and interpolates the per-row
 *      "force" anchor positions (attachment_force_points) by lerping between consecutive attachment points
 *      across the rows each attachment spans;
 *   2. if the first attachment point has jumped by a whole world unit on any axis since last update, rigidly
 *      translates every cloth vertex by that delta so the flag follows the pole instead of snapping taut;
 *   3. records the new first attachment point.
 *
 * Sibling of antenna_update_attachment (same marker resolve + whole-unit-jump rigid-translate idiom, same
 * per-vertex +28 position layout; cloth vertices use a 24-byte stride from flag+0x1C).
 *
 * DEVIATIONS: (1) the layout loop's __int64 HIDWORD/LODWORD registers are plain int16 loop counters
 * (attachment index, row_y, end_y, cur_y) — decoded from disasm 0x8380E0C0-0x8380E220; the lerp parameter is
 * t = (cur_y - row_y) / (end_y - row_y). (2) the per-axis "big jump" test is abs32((int)delta) > 1.0 =
 * truncate-to-int, integer abs, back to float, compare > 1.0 (disasm 0x8380E248-0x8380E2E0, NOT fabsf) — so
 * only a whole-unit move of >= 2 triggers; reproduced faithfully. */

#include <stdint.h>
#include "headers/flag_datum.h"
#include "headers/flag_definition.h"
#include "headers/flag_attachment_point.h"
#include "headers/location.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"

extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void scenario_location_from_point(location *location, const real_point3d *point);

void flag_update_attachment(flag_datum *flag, flag_definition *flag_definition, location *attachment_location, real_point3d *attachment_points, real_point3d *attachment_force_points, int16_t *attachment_y, int16_t *y_attachments)
{
    /* Resolve each attachment marker's object-space position. */
    for ( int i = 0; i < flag_definition->attachment_points.count; i = (int16_t)(i + 1) )
    {
        object_marker marker;
        object_get_marker_by_name(flag->object_index,
                ((flag_attachment_point *)flag_definition->attachment_points.address)[i].marker_name, &marker, 1);
        attachment_points[i].n[0] = marker.matrix.position.n[0];
        attachment_points[i].n[1] = marker.matrix.position.n[1];
        attachment_points[i].n[2] = marker.matrix.position.n[2];
    }

    scenario_location_from_point(attachment_location, attachment_points);

    if ( flag->__noop )
        return;

    /* Reset the row→attachment map. */
    for ( int row = 0; row < flag_definition->height; row = (int16_t)(row + 1) )
        y_attachments[row] = -1;

    /* Build the row maps and interpolate the per-row force anchors. */
    int16_t attachment_index = 0;
    int16_t row_y = 0;
    while ( attachment_index < flag_definition->attachment_points.count )
    {
        if ( row_y >= flag_definition->height )
            break;

        int span = ((flag_attachment_point *)flag_definition->attachment_points.address)[attachment_index].height_to_next_attachment;
        if ( span < 0 )
            span = 0;
        else if ( span > flag_definition->height - row_y )
            span = flag_definition->height - row_y;

        int16_t end_y = (int16_t)((span & ~0x1u) + row_y);   /* span rounded down to even */
        attachment_y[attachment_index] = row_y;
        y_attachments[row_y] = attachment_index;

        if ( row_y <= end_y )
        {
            for ( int16_t cur_y = row_y; cur_y <= end_y; cur_y = (int16_t)(cur_y + 1) )
            {
                if ( end_y != row_y )
                {
                    float t = (float)(cur_y - row_y) / (float)(end_y - row_y);
                    real_point3d *from = &attachment_points[attachment_index];
                    real_point3d *to = &attachment_points[attachment_index + 1];
                    real_point3d *dest = &attachment_force_points[cur_y];
                    dest->n[0] = (to->n[0] * t) + (from->n[0] * (1.0f - t));
                    dest->n[1] = (from->n[1] * (1.0f - t)) + (to->n[1] * t);
                    dest->n[2] = (from->n[2] * (1.0f - t)) + (to->n[2] * t);
                }
            }
        }

        attachment_index = (int16_t)(attachment_index + 1);
        row_y = end_y;   /* post-loop cur_y (end_y+1) minus 1 */
    }

    /* Rigid-translate the cloth if the anchor jumped a whole world unit (truncated-int abs > 1.0). */
    float delta_x = attachment_points->n[0] - flag->first_attachment.n[0];
    float delta_y = attachment_points->n[1] - flag->first_attachment.n[1];
    float delta_z = attachment_points->n[2] - flag->first_attachment.n[2];

    int moved_x = (int)delta_x;
    int moved_y = (int)delta_y;
    int moved_z = (int)delta_z;
    if ( moved_x < 0 ) moved_x = -moved_x;
    if ( moved_y < 0 ) moved_y = -moved_y;
    if ( moved_z < 0 ) moved_z = -moved_z;

    if ( (float)moved_x > 1.0f || (float)moved_y > 1.0f || (float)moved_z > 1.0f )
    {
        for ( int16_t col = 0; col < flag_definition->width; col = (int16_t)(col + 1) )
        {
            for ( int16_t row = 0; row < flag_definition->height; row = (int16_t)(row + 1) )
            {
                int index = col * flag_definition->height + row;
                /* recovered: (char *)&flag->identifier + 24*index + 28 -> flag->vertices[index].p */
                real_point3d *vertex_position = &flag->vertices[index].p;
                vertex_position->n[0] = vertex_position->n[0] + delta_x;
                vertex_position->n[1] = vertex_position->n[1] + delta_y;
                vertex_position->n[2] = vertex_position->n[2] + delta_z;
            }
        }
    }

    flag->first_attachment.n[0] = attachment_points->n[0];
    flag->first_attachment.n[1] = attachment_points->n[1];
    flag->first_attachment.n[2] = attachment_points->n[2];
}
