/* game_engine_should_spawn_player @0x837473C8 — decides whether a player should (re)spawn this tick.
 * A player that has never spawned spawns immediately; one that has quit, run out of lives, is the odd
 * man out, or is in a post-game state never spawns. Otherwise it counts down the respawn timer (playing
 * the countdown beeps at 90/60/30/1 ticks for local players) and spawns when it reaches zero. When a
 * spawn is due more than 3 ticks into the game, it is additionally gated to a per-player time slot to
 * stagger simultaneous respawns.
 *
 * DEVIATION: the decompiler passes an uninitialised `v6` to game_engine_player_is_odd_man_out; the
 * register is player_index (the only candidate), used here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_engine_globals.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"

struct game_engine;

extern struct game_engine     *game_engine;

extern uint8_t game_engine_player_is_out_of_lives(int player_index);
extern uint8_t game_engine_player_is_odd_man_out(int player_index);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern int game_time_get(void);

uint8_t game_engine_should_spawn_player(int player_index)
{
    if (!game_engine)
        return 0;

    int player_slot = (uint16_t)player_index;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    int should_spawn;
    if (player->quit_out_of_game == 1)
    {
        should_spawn = 0;
    }
    else if (player->statistics.deaths)   /* has died before => this is a respawn */
    {
        if (!game_engine_player_is_out_of_lives(player_index)
            && !game_engine_player_is_odd_man_out(player_index)
            && game_engine_globals.mode != game_engine_mode_postgame_rasterize
            && game_engine_globals.mode != game_engine_mode_postgame_rasterize_delay)
        {
            int respawn_timer = player->respawn_timer;
            if (respawn_timer <= 0)
            {
                should_spawn = 1;
            }
            else
            {
                if (player->local_player_index != -1)   /* local player */
                {
                    switch (respawn_timer)
                    {
                        case 90:
                        case 60:
                        case 30:
                            game_engine_play_multiplayer_sound(_multiplayer_sound_countdown_for_respawn, 0);
                            break;
                        case 1:
                            game_engine_play_multiplayer_sound(_multiplayer_sound_respawn, 0);
                            break;
                    }
                }
                int new_timer = player->respawn_timer - 1;
                player->respawn_timer = new_timer;
                should_spawn = (new_timer == 0);
            }
        }
        else
        {
            should_spawn = 0;
        }
    }
    else
    {
        should_spawn = 1;
    }

    if (should_spawn && game_time_get() > 3)
        return player_slot % 32 == game_time_get() % 32 && should_spawn;
    return should_spawn;
}
