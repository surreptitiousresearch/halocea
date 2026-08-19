/* light_new_unattached @0x836F93C8 — allocate a dynamic light datum and place it, either free-standing in
 * the world (object_index == -1: position+direction stored directly) or attached to an object node. Stamps
 * the creation time and scale, marks the datum active (flags |= 3), reconnects it to the BSP, and records
 * the current lights marker. Returns the light datum index, or -1 if none was free.
 *
 * For an attached light the node transform is stored in the datum's per-node scratch region (offset 0x60..),
 * which the light_datum header leaves opaque, so those six stores are kept as faithful index writes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/lights_globals.h"
#include "headers/point_light_flags.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern int game_time_get(void);
extern void light_reconnect_to_map(int light_index);

int light_new_unattached(int definition_index, int object_index, int16_t node_index,
        real_point3d *position, real_vector3d *forward, float scale)
{
    int index = datum_new(light_data);
    light_datum *record;
    int16_t flags;

    if ( index == -1 )
        return index;

    record = DATA_ARRAY_ELEMENT(light_data, light_datum, index);
    record->flags = 0;
    flags = record->flags;
    record->intensity_scale = scale;
    record->parent_light_index = game_time_get();   /* dual-purposed as creation timestamp */
    record->definition_index = definition_index;
    record->object_index = object_index;
    record->flags = flags | (1u << _point_light_dynamic_bit) | (1u << _point_light_connects_to_map_bit); /* | 3 */
    record->cluster_reference = -1;

    if ( object_index == -1 )
    {
        record->position.n[0] = position->n[0];
        record->position.n[1] = position->n[1];
        record->position.n[2] = position->n[2];
        record->forward.n[0] = forward->n[0];
        record->forward.n[1] = forward->n[1];
        record->forward.n[2] = forward->n[2];
    }
    else
    {
        record->attachment_marker_index = node_index;
        record->relative_position.n[0] = position->n[0];
        record->relative_position.n[1] = position->n[1];
        record->relative_position.n[2] = position->n[2];
        record->relative_forward.n[0] = forward->n[0];
        record->relative_forward.n[1] = forward->n[1];
        record->relative_forward.n[2] = forward->n[2];
    }

    light_reconnect_to_map(index);
    record->marker = lights_globals.marker - 1;
    return index;
}
