/* update_ball_ownership @0x83817160 — oddball engine: called when a player's ball possession may have
 * changed. Skipped entirely outside an oddball-family variant (ball type <= normal, or > terminator).
 * Otherwise, first clears this player from every team's current_ball_owner slot; then, if their unit is
 * currently holding a flag-type weapon in its inventory, records this player as that flag object's owning
 * team's current ball owner. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/oddball_globals.h"
#include "headers/unit_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern uint8_t weapon_is_flag(int weapon_index);

void update_ball_ownership(int player_index)
{
    oddball_ball_type ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    if ( ball_type <= _oddball_normal || ball_type > _oddball_terminator )
        return;

    for ( int team = 0; team < 16; team++ )
    {
        if ( oddball_globals.current_ball_owner[team] == player_index )
            oddball_globals.current_ball_owner[team] = -1;
    }

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    if ( player->unit_index == -1 )
        return;

    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;
    __int16 current_weapon_slot = unit_object->unit.current_weapon_index;
    if ( current_weapon_slot == -1 )
        return;

    int weapon_index = unit_object->unit.weapon_object_indices[current_weapon_slot];
    if ( weapon_index == -1 || !weapon_is_flag(weapon_index) )
        return;

    object_datum *weapon_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    oddball_globals.current_ball_owner[weapon_object->object.owner_team_index] = player_index;
}
