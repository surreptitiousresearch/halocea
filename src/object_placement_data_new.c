/* object_placement_data_new @0x836EED88 — initialize an object_placement_data request with its defaults
 * for the given object definition. Sets the world-aligned forward/up basis, copies owner identity from the
 * owning object's unit datum when present (player/team index), and initializes all four change-color slots
 * to white. */

#include "headers/object_placement_data.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern void *memset(void *dst, int value, unsigned int n);

void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index)
{
    object_header_datum *owner_header;
    int i;

    memset(data, 0, sizeof(*data));   /* decompiler wrote sizeof on the type; real struct size */
    data->definition_index = definition_index;
    data->flags = 0;
    data->forward = *global_forward3d;
    data->up = *global_up3d;
    data->variant_number = 0;

    owner_header = datum_try_and_get(object_header_data, owner_object_index);
    /* (1 << type) is non-zero only for a valid object type (< 32) */
    if ( owner_header && (1 << owner_header->type) && owner_header->datum )
    {
        const object_datum *owner_object = owner_header->datum;
        data->owner_object_index = owner_object_index;
        data->owner_player_index = owner_object->object.owner_player_index;
        data->owner_team_index = owner_object->object.owner_team_index;
    }
    else
    {
        data->owner_object_index = -1;
        data->owner_player_index = -1;
        data->owner_team_index = -1;
    }

    for ( i = 0; i < 4; ++i )
        data->change_colors[i] = *global_real_rgb_white;
}
