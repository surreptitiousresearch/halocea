/* item_in_unit_inventory @0x83757CC0 — update an item object's inventory state when it is picked up by (or
 * removed from) a unit. When detached (owner == -1) it clears the "in inventory" item.flags low bits.
 * When attached it marks the item as held (bit 0x01, plus bit 0x02 if the owner is player-controlled),
 * copies the owner's unit.player_index into the item's object.owner_player_index, un-garbages it, clears its
 * BSP object.location, awards any location pickup bonus, and — if the item's object.datum_role is 0 —
 * forces a network update (object.flags bit 0x4000000). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/location.h"
#include "headers/item_datum.h"
#include "headers/unit_datum.h"
#include "headers/item_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern void object_set_garbage(int object_index, uint8_t garbage);
extern void scenario_location_award_bonus(location *location);

void item_in_unit_inventory(int item_index, int owner_unit_index)
{
    item_datum *item = (item_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;

    if ( owner_unit_index == -1 )
    {
        /* detached: clear held + player-controlled */
        item->item.flags &= ~((1u << _item_in_unit_inventory_bit) | (1u << _item_belongs_to_player_bit));
        return;
    }

    unsigned int flags = item->item.flags;
    unit_datum *owner = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, owner_unit_index)->datum;
    /* dead store (shipped): immediately overwritten by the player-index branch below */
    item->item.flags = flags & ~((1u << _item_in_unit_inventory_bit) | (1u << _item_part_of_respawn_system))
                     | (1u << _item_in_unit_inventory_bit);
    if ( owner->unit.player_index == -1 )
        /* held by a non-player unit: mark in-inventory only */
        item->item.flags = flags & ~((1u << _item_in_unit_inventory_bit) | (1u << _item_belongs_to_player_bit)
                                     | (1u << _item_part_of_respawn_system))
                         | (1u << _item_in_unit_inventory_bit);
    else
        /* held by a player-controlled unit: mark in-inventory + belongs-to-player */
        item->item.flags = flags & ~((1u << _item_in_unit_inventory_bit) | (1u << _item_belongs_to_player_bit)
                                     | (1u << _item_part_of_respawn_system))
                         | (1u << _item_in_unit_inventory_bit) | (1u << _item_belongs_to_player_bit);
    item->object.owner_player_index = owner->unit.player_index;

    object_set_garbage(item_index, 0);

    unsigned int held_flags = item->item.flags;
    item->object.location.leaf_index = -1;
    item->object.location.cluster_index = -1;
    /* dropped from the BSP: clear on-structure + does-not-accelerate resting state */
    item->item.flags = held_flags & ~((1u << _item_on_structure_bit) | (1u << _item_does_not_accelerate_bit));
    scenario_location_award_bonus(&item->object.location);

    if ( !item->object.datum_role )
        item->object.flags |= (1u << _object_force_baseline_update_bit);
}
