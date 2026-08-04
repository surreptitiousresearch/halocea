/* remove_quitting_players_from_game @ 0x83687E70 — once the game engine is running, scan players and
 * retire any that have quit. A player is retired when its quit-game time is set, it hasn't already been
 * processed, and either we are a network client or its quit time has arrived. Retiring a player with no
 * attached unit runs the full cleanup (and restarts the iteration, since cleanup mutates the array);
 * a player still attached to a unit just has that unit killed (without crediting statistics).
 *
 * Deviation: the decompiler's void* return (the last iterator result) is discarded; this is void. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_running(void);
extern int game_time_get(void);
extern int16_t game_connection(void);
extern void player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(int player_index);
extern void unit_kill_no_statistics(int unit_index);

void remove_quitting_players_from_game(void)
{
    data_iterator iter;
    int current_time;
    player_datum *player;

    if ( !game_engine_running() )
        return;

    current_time = game_time_get();
    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player;
          player = data_iterator_next(&iter) )
    {
        if ( player->quit_out_of_game_time != -1
          && !player->quit_out_of_game
          && (game_connection() == _game_connection_network_client || player->quit_out_of_game_time == current_time) )
        {
            int unit_index = player->unit_index;
            player->quit_out_of_game = 1;
            if ( unit_index == -1 )
            {
                player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(iter.index);
                data_iterator_new(&iter, player_data);   /* cleanup mutated the array: restart */
            }
            else
            {
                unit_kill_no_statistics(unit_index);
            }
        }
    }
}
