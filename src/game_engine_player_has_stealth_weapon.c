/* game_engine_player_has_stealth_weapon @0x83749ADC — true if the player's unit's currently-selected
 * weapon (player unit_index -> unit.current_weapon_index -> unit.weapon_object_indices slot -> weapon
 * object's tag) has the "stealth" weapon flag (weapon.flags bit 13) set. False at any missing link in
 * the chain (no unit, no selection, no weapon, no tag). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


uint8_t game_engine_player_has_stealth_weapon(int player_index)
{
    if ( player_index == -1 )
        return 0;

    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    if ( unit_index == -1 )
        return 0;

    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int16_t selected_index = unit->unit.current_weapon_index;
    if ( selected_index == -1 )
        return 0;

    int weapon_object_index = unit->unit.weapon_object_indices[selected_index];
    if ( weapon_object_index == -1 )
        return 0;

    object_datum *weapon_object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_object_index)->datum;
    int weapon_tag_index = weapon_object->definition_index;
    if ( weapon_tag_index == -1 )
        return 0;

    weapon_definition *definition = TAG_GET(weapon_definition, weapon_tag_index);
    return (definition->weapon.flags >> _weapon_does_not_depower_active_camo_bit) & 1;
}
