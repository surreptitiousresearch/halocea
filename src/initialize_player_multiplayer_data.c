/* initialize_player_multiplayer_data @0x83746008 — resets a player's speed multiplier (+108 = 1.0) and
 * clears its "last killer/victim" index fields (+112/+116/+120/+124) and a counter (+196), mirroring the
 * reset done in game_engine_player_added.
 *
 * DEVIATION: the decompiler renders the -1 index stores at +112/+116/+120/+124 as float NAN; disasm shows
 * plain `stw` of -1 (the "NONE" sentinel), modelled as int here (same pattern as game_engine_player_added). */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void initialize_player_multiplayer_data(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    player->multiplayer.speed_multiplier = 1.0f;
    player->multiplayer.state_message = -1;
    player->multiplayer.state_message_data = -1;
    player->multiplayer.teleporter_index = -1;
    player->multiplayer.player_display_index = -1;
    /* DEVIATION: the binary's single stw clears the first two int16 stats in one word;
     * transcribed as the two member clears it performs */
    player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs = 0;
    player->statistics.multiplayer_statistics.ctf_statistics.flag_returns = 0;
}
