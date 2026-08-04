/* game_engine_update_player_always_invis @0x8374D2C0 — keep a player permanently cloaked when the
 * active-camo "always on" condition holds: either the universal variant's always-invis flag (0x10) is
 * set, or the game engine's trait test grants invisibility (trait 1) to this player. When so, and the
 * player has a living unit (player datum +52), refresh the minor active-camouflage powerup (type 15). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/game_engine_trait.h"
#include "headers/blam_data_globals.h"

extern void player_handle_powerup_minor(int player_index, int16_t powerup_type, int16_t minor_duration);

void game_engine_update_player_always_invis(int player_index)
{
    if ( game_engine )
    {
        unsigned char grant = (global_variant.universal_variant.flags & (1u << _always_invisible_bit)) != 0;
        if ( !grant && game_engine->game_engine_test_trait )
            grant = game_engine->game_engine_test_trait(player_index, _game_trait_invisible);
        if ( grant )
        {
            if ( DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index != -1 )
                player_handle_powerup_minor(player_index, 0, 15);
        }
    }
}
