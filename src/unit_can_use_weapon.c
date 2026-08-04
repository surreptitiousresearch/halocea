/* unit_can_use_weapon @ 0x836CF578 — decide whether a unit is allowed to pick up / wield a weapon. The
 * weapon's label must be compatible with the unit's current seat (unit_set_or_test_seat_and_weapon_label),
 * the game engine must allow the pickup, and — under the HCEX "no covenant weapons" mod — covenant
 * weapon_type values (_weapon_type_needler.._weapon_type_plasma_rifle) are rejected while the unit is
 * player-owned (object.owner_player_index != -1). Returns nonzero when the weapon may be used. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_type.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern const char *unit_get_seat_label(int object_index);
extern const char *weapon_get_label(int weapon_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern uint8_t game_engine_allow_pick_up(int unit_index, int item_index);

uint8_t unit_can_use_weapon(int unit_index, int weapon_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    object_datum *weapon = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    const char *seat_label = unit_get_seat_label(unit_index);
    const char *weapon_label = weapon_get_label(weapon_index);
    int result = unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, weapon_label, 0);
    if ( (unsigned char)result )
        result = game_engine_allow_pick_up(unit_index, weapon_index);

    if ( unit->object.owner_player_index != -1 && (unsigned char)result && !hcex_allow_covenant_weapon )
    {
        unsigned int weapon_type = TAG_GET(weapon_definition, weapon->definition_index)->weapon.weapon_type;
        /* covenant weapons: needler(2), plasma_pistol(3), plasma_rifle(4) — DB enum $576107C8 */
        if ( weapon_type >= _weapon_type_needler )
            return weapon_type <= _weapon_type_plasma_rifle ? 0 : result;
    }
    return result;
}
