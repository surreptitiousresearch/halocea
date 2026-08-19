/* player_score @0x83806448 — CTF scoring handler: credit a flag capture to `team_index` and the capturing
 * player (per-player score word at player+200, the same field ctf_get_score reads), replicate the engine
 * score/state when hosting, play the team-dependent "flag captured" multiplayer sound (red vs blue),
 * and flash the you/ally/enemy scored CTF HUD messages. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/ctf_globals.h"
#include "headers/iterated_message.h"
#include "headers/game_connection.h"
#include "headers/multiplayer_team.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/ctf_message.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern void game_engine_replicate_score_and_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate);

void player_score(int player_index, int team_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    ++ctf_globals.score[team_index];
    ++player->statistics.multiplayer_statistics.ctf_statistics.flag_scores;  /* flag_scores in the DB ctf_statistics union view */

    if ( game_connection() == _game_connection_network_server )
        game_engine_replicate_score_and_state_to_network(_message_delta_mode_incremental, -1);

    game_engine_play_multiplayer_sound(
        player->team_index == _multiplayer_team_red
            ? _multiplayer_sound_ctf_red_captured_flag
            : _multiplayer_sound_ctf_blue_captured_flag,
        1u);
    game_show_score_you_ally_enemy(player_index, ctf_message_you_scored, ctf_message_good_guys_scored,
            ctf_message_enemy_scored, player_index, 1u);
}
