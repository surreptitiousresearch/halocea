/* ctf_engine_allow_pick_up @0x83806670 — CTF pickup gate for `item_index` by `unit_index`'s player. The
 * rule is: you may not pick up your OWN team's flag. Only a network server (game_connection() == 2,
 * `cmpwi cr6,r11,2 / bne @0x838066A4-A8`) gates at all; every other path returns 1 (allow).
 * On the server, for an item that weapon_is_flag() accepts:
 *   - _weapon_multiplayer_flag (weapon.flags @ item+0x22C, bit 0x40) SET => allow
 *     (`rlwinm r10,r11,0,25,25` / `bne cr6,loc_83806720` @0x838066F0-F8 branches to the return-1 epilogue);
 *   - otherwise the result is `player.team_index != item->object.owner_team_index`
 *     (`lhz r11,0xB8(r31)` / `subf r8,r9,r10` / `subfic r7,r8,0` / `subfe r5,r6,r6` / `and r3,r5,r28`
 *     @0x838066FC-0x83806714 — r5 is 0 when the difference is zero), so a MATCHING team is the denial.
 * Non-flag items, unresolvable items, and local/client play are always allowed.
 * (The previous banner stated both clauses inverted and invented the label "cannot be picked up" for
 * bit 0x40; that reading is what the derived documentation copied.) */

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
