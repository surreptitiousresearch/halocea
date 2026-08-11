/* slayer_engine_player_killed_player @0x83815680 — slayer scoring on a kill: if the dead player wasn't
 * already marked dead (player record +213, opaque flag byte) and there's a real killer, either penalize a
 * friendly-fire kill (-1) or credit a normal kill (+1, after updating momentum-based speed and, in CTF with
 * a "flag must reset" variant on a dedicated server, redirecting the flag carrier's target if the dead
 * player was it — killer record +136, opaque object/unit index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern void slayer_engine_adjust_score(int player_index, int adjustment);
extern void update_speed_for_score(int dead_player_index, int killing_player_index);
extern game_variant *game_engine_get_variant(void);
extern int16_t game_connection(void);
extern void find_next_target(int my_index);

void slayer_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire)
{
    if ( DATA_ARRAY_ELEMENT(player_data, player_datum, dead_player_index)->quit_out_of_game || killing_player_index == -1 )
        return;

    player_datum *killer = DATA_ARRAY_ELEMENT(player_data, player_datum, killing_player_index);

    if ( friendly_fire )
    {
        slayer_engine_adjust_score(killing_player_index, -1);
        return;
    }

    update_speed_for_score(dead_player_index, killing_player_index);

    if ( game_engine_get_variant()->game_engine_variant.ctf.flag_must_reset && game_connection() == _game_connection_network_server )
    {
        if ( killer->multiplayer.special != dead_player_index )
            return;
        find_next_target(killing_player_index);
    }

    slayer_engine_adjust_score(killing_player_index, 1);
}
