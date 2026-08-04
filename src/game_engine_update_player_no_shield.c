/* game_engine_update_player_no_shield @0x8374D20C — if the active game engine's variant disables shields
 * (universal_variant.flags bit 3 clear means shields ARE disabled; no active game engine defaults to
 * "no shield"), zeroes the player's unit's shield/current-shield-damage floats (unit +220/+228). Player
 * record dword +52 is the controlled unit index (see any_player_is_dead.c). */

#include "headers/data_array.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

void game_engine_update_player_no_shield(int player_index)
{
    unsigned char no_shield = 1;

    if ( game_engine && player_index != -1 )
        no_shield = (~global_variant.universal_variant.flags >> _no_shields_bit) & 1;

    if ( !no_shield )
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int unit_index = player->unit_index;
        if ( unit_index != -1 )
        {
            unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
            unit->object.shield_vitality = 0.0f;
            unit->object.maximum_shield_vitality = 0.0f;
        }
    }
}
