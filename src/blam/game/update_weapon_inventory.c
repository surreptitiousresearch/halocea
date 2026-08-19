/* update_weapon_inventory @0x83746B10 — when a weapon with no owner (parent_object_index == -1) is flagged
 * for multiplayer inventory (weapon.flags _weapon_multiplayer_inventory_flag, bit5) and is not already held
 * in a unit inventory (item.flags _item_in_unit_inventory_bit), clear the inventory flag and run the game
 * engine's weapon_drop hook.
 *
 * DEVIATION: disasm_range(0x83746B10,0x83746B90) ends in a genuine tail call (`bctr`, not `bctrl`) with r3
 * never reassigned after entry — so the original `weapon_index` parameter is passed through as the hook's
 * argument, matching game_engine.h's own `weapon_drop` field type (`void (__fastcall *)(int)`); the
 * decompiler dropped this argument, rendering a zero-arg call. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/game_engine.h"
#include "headers/weapon_datum.h"
#include "headers/item_flags.h"
#include "headers/weapon_datum_flags.h"
#include "headers/blam_data_globals.h"


void update_weapon_inventory(int weapon_index)
{
    weapon_datum *object = ((weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum);

    if ( object->object.parent_object_index == -1 && (object->item.flags & (1u << _item_in_unit_inventory_bit)) == 0 )
    {
        int flags = object->weapon.flags;

        if ( flags & (1u << _weapon_multiplayer_inventory_flag) )
        {
            object->weapon.flags = flags & ~(1u << _weapon_multiplayer_inventory_flag);
            if ( game_engine->weapon_drop )
                game_engine->weapon_drop(weapon_index);
        }
    }
}
