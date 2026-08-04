/* game_engine_weapon_fired @0x83749B90 — notify the game engine a player's weapon fired, dinging the
 * unit's active camouflage: no-op unless a game engine is active, the player has a unit, and that unit has
 * a current inventory weapon. Determines the ding (default 0.1, forced to 0.0 for a "stealth" weapon, or
 * the weapon definition's active_camo_ding when nonzero) and, once unit.active_camouflage is at least
 * 0.05, subtracts the ding, sets unit.cause_for_camo_regrowth, and clamps to a floor of 0.05.
 * (The old "autoaim/assist decay" description was wrong — DB member names are the camo fields.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t game_engine_player_has_stealth_weapon(int player_index);

void game_engine_weapon_fired(int player_index)
{
    if ( !game_engine || player_index == -1 )
        return;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;
    if ( unit_index == -1 )
        return;

    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int weapon_index = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);

    float decay = 0.1f;
    if ( game_engine_player_has_stealth_weapon(player_index) )
    {
        decay = 0.0f;
    }
    else if ( weapon_index != -1 )
    {
        object_datum *weapon_object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
        weapon_definition *definition =
            TAG_GET(weapon_definition, weapon_object->definition_index);
        float weapon_decay = definition->weapon.active_camo_ding;
        if ( weapon_decay != 0.0f )
            decay = weapon_decay;
    }

    if ( unit->unit.active_camouflage >= 0.05f )
    {
        float camo = unit->unit.active_camouflage - decay;
        unit->unit.active_camouflage = camo;
        unit->unit.cause_for_camo_regrowth = 1;
        if ( camo < 0.05f )
            camo = 0.05f;
        unit->unit.active_camouflage = camo;
    }
}
