/* race_complete_lap @0x8382A3B8 — race engine: a player finishes a lap. Resets their per-flag lap bit
 * vector, plays the lap-complete sound, records this lap's elapsed time (since the last touch, tracked in
 * the reused multiplayer.special scratch field) as the lap timer, updates the best-lap time if this lap
 * beat it (or unconditionally on the very first lap), bumps the lap count, resets the touch timer, and
 * updates team score / HUD score messages — an extended "personal best" message additionally fires outside
 * rally-race variants. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/race_globals.h"
#include "headers/game_variant.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/race_message.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void game_engine_play_multiplayer_sound_to_specific_player(int player_index, int index, uint8_t should_replicate);
extern game_variant *race_update_team_score(void);
extern game_variant *game_engine_get_variant(void);
extern void game_show_score_you_ally_enemy(int player_index, int message_you, int message_ally, int message_enemy, int message_data, uint8_t should_replicate);
extern void game_show_score_extended(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void race_complete_lap(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* the multiplayer_statistics union viewed as race_statistics: last_lap_time@+196, laps@+198,
     * best_lap_time@+200 (DB: race_statistics {last_lap_time@0, laps@2, best_lap_time@4}) */
    race_statistics *race_stats = &player->statistics.multiplayer_statistics.race_statistics;

    unsigned __int8 new_best_lap = 0;
    int lap_time = game_time_get() - player->multiplayer.special;

    race_globals.lap_bit_vector[player_index] = 0;
    game_engine_play_multiplayer_sound_to_specific_player(player_index, _multiplayer_sound_countdown_timer_end, 1u);

    __int16 previous_lap_count = race_stats->laps;
    race_stats->last_lap_time = (__int16)lap_time;

    if ( !previous_lap_count || lap_time < race_stats->best_lap_time )
    {
        race_stats->best_lap_time = (__int16)lap_time;
        if ( previous_lap_count )
            new_best_lap = 1;
    }

    race_stats->laps = previous_lap_count + 1;
    player->multiplayer.special = game_time_get();

    race_update_team_score();

    int message_ally, message_you;
    if ( game_engine_get_variant()->game_engine_variant.terminator.ignored == 2 ) /* rally mode */
    {
        message_ally = race_message_ally_touched_a_flag_rally;
        message_you = race_message_you_touched_a_flag_rally;
    }
    else
    {
        message_ally = race_message_ally_completed_a_lap;
        message_you = race_message_you_completed_a_lap;
    }
    /* enemy id stays race_message_enemy_completed_a_lap (33) even in rally mode (shipped) */
    game_show_score_you_ally_enemy(player_index, message_you, message_ally,
            race_message_enemy_completed_a_lap, player_index, 1u);

    if ( game_engine_get_variant()->game_engine_variant.terminator.ignored != 2 && new_best_lap == 1 )
        game_show_score_extended(-1, player_index, race_message_new_best_lap_time, player_index, 1u);

    game_engine_get_variant();
}
