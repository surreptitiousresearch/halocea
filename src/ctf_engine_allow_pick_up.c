/* ctf_engine_allow_pick_up @0x83806670 — CTF pickup gate for `item_index` by `unit_index`'s player: always
 * allowed except on a dedicated server (game_connection() == _game_connection_network_server) for a flag whose "cannot be picked up" flag
 * (item+556, bit 0x40) is set or whose owning team (object.owner_team_index @ item+184, a signed halfword)
 * doesn't match the would-be picker's team (player_datum.team_index @ +32). Non-flag items, unowned items, and
 * local/non-networked play are always allowed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/game_connection.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int player_index_from_unit_index(int unit_index);
extern int16_t game_connection(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_is_flag(int weapon_index);

uint8_t ctf_engine_allow_pick_up(int unit_index, int item_index)
{
    int player_index = player_index_from_unit_index(unit_index);
    if ( player_index == -1 )
        return 1;
    if ( item_index == -1 )
        return 1;
    if ( game_connection() != _game_connection_network_server )
        return 1;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    weapon_datum *item = object_try_and_get_and_verify_type(item_index, object_mask_weapon);

    /* disasm 0x838066FC lhz 0xB8 (item+184 = object.owner_team_index, signed halfword), NOT item[92]/+368 */
    return !item || !weapon_is_flag(item_index) || (item->weapon.flags & (1u << _weapon_multiplayer_flag)) != 0
            || player->team_index != item->object.owner_team_index;
}
