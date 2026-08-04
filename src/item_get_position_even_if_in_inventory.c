/* item_get_position_even_if_in_inventory @0x83758178 — get an item's world position even when it's
 * currently stowed in a player's inventory: if the item exists (type mask 0x1C) and isn't flagged "in
 * inventory" (item.flags bit 0 = _item_in_unit_inventory_bit), returns its own position (floats +160/164/168). Otherwise, if it
 * has a holder player (object dword +192) whose unit exists (player datum +52, per player_datum.unit_index),
 * returns the holder unit's position instead. `position` is always zeroed first; returns whether it was
 * resolved. */

#include "headers/data_array.h"
#include "headers/item_flags.h"
#include "headers/real_point3d.h"
#include "headers/item_datum.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);

int item_get_position_even_if_in_inventory(int item_index, real_point3d *position)
{
    item_datum *item = object_try_and_get_and_verify_type(item_index, object_mask_item);

    position->n[0] = 0.0f;
    position->n[1] = 0.0f;
    position->n[2] = 0.0f;

    if ( !item )
        return 0;

    if ( (item->item.flags & (1u << _item_in_unit_inventory_bit)) == 0 )
    {
        position->n[0] = item->object.bounding_sphere_center.n[0];
        position->n[1] = item->object.bounding_sphere_center.n[1];
        position->n[2] = item->object.bounding_sphere_center.n[2];
        return 1;
    }

    int holder_player_index = item->object.owner_player_index;
    if ( holder_player_index == -1 )
        return 0;

    player_datum *player = datum_try_and_get(player_data, holder_player_index);
    if ( !player )
        return 0;

    int holder_unit_index = player->unit_index;
    if ( holder_unit_index == -1 )
        return 0;

    object_datum *holder_unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, holder_unit_index)->datum;
    position->n[0] = holder_unit->object.bounding_sphere_center.n[0];
    position->n[1] = holder_unit->object.bounding_sphere_center.n[1];
    position->n[2] = holder_unit->object.bounding_sphere_center.n[2];
    return 1;
}
