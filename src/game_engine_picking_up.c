/* game_engine_picking_up @0x837481FC — let the active multiplayer game mode react to a unit picking up a
 * weapon item, and decide whether the pickup is allowed. With no game engine (single-player) the pickup is
 * always allowed. Only flag weapons are handled: if the item was already flagged as picked up
 * (_weapon_multiplayer_inventory_flag, bit 5), it is first dropped via the game engine's weapon_drop
 * callback. The item is then re-marked picked
 * up and, if the game engine has a weapon_pickup callback, its result decides whether the pickup proceeds.
 * Returns 1 (allowed) in the absence of a game engine, non-flag item, or pickup callback. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/object_type.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_is_flag(int weapon_index);
extern int player_index_from_unit_index(int unit_index);

uint8_t game_engine_picking_up(int unit_index, int item_index)
{
    if (!game_engine)
        return 1;

    weapon_datum *weapon = object_try_and_get_and_verify_type(item_index, object_mask_weapon);
    if (!weapon || !weapon_is_flag(item_index))
        return 1;

    int flags = weapon->weapon.flags;
    if ((flags & (1u << _weapon_multiplayer_inventory_flag)) != 0)
    {
        weapon->weapon.flags = flags & ~(1u << _weapon_multiplayer_inventory_flag);
        if (game_engine->weapon_drop)
            game_engine->weapon_drop(item_index);
    }
    weapon->weapon.flags |= (1u << _weapon_multiplayer_inventory_flag);

    if (!game_engine->weapon_pickup)
        return 1;

    int player_index = player_index_from_unit_index(unit_index);
    return game_engine->weapon_pickup(item_index, player_index);
}
