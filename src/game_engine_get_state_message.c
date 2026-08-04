/* game_engine_get_state_message @0x8374E460 — build a multiplayer state-change notification message
 * for a player, for display on that player's screen. Clears a stale pending-message id (odd_man_out..
 * waiting_for_space_to_clear) so it isn't resent. If the player has a live unit, defers to a currently-
 * queued message (or, in the first 450 ticks of the game, always sends press_back_for_score). Otherwise
 * picks a reason: player_quit_self, out_of_lives, odd_man_out, waiting_for_space_to_clear (no timer) or
 * respawn_timer (with the remaining seconds as message data). Returns false if no game_engine is active. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_engine_message.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern uint8_t game_engine_player_is_out_of_lives(int player_index);
extern uint8_t game_engine_player_is_odd_man_out(int player_index);
extern uint8_t multiplayer_message_internal(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);

uint8_t game_engine_get_state_message(int player_index, uint16_t *buffer, int buffer_length)
{
    if ( !game_engine )
        return 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* pending state message id/data — multiplayer_player_info.state_message (+0x74) / state_message_data (+0x78) */
    /* recovered: (int *)((char *)player + 116) -> &player->multiplayer.state_message */
    int *pending_message = &player->multiplayer.state_message;
    if ( *pending_message >= game_engine_message_odd_man_out
      && *pending_message <= game_engine_message_waiting_for_space_to_clear )
        *pending_message = -1;

    if ( player->unit_index != -1 )
    {
        if ( game_time_get() < 450 )
            return multiplayer_message_internal(player_index, game_engine_message_press_back_for_score, -1, buffer, buffer_length);

        if ( *pending_message == -1 )
            return 0;

        int *pending_message_data = &player->multiplayer.state_message_data;
        return multiplayer_message_internal(player_index, *pending_message, *pending_message_data, buffer, buffer_length);
    }

    int message;
    int message_data = 0;

    if ( player->quit_out_of_game == 1 )
        message = game_engine_message_player_quit_self;
    else if ( game_engine_player_is_out_of_lives(player_index) )
        message = game_engine_message_out_of_lives;
    else if ( game_engine_player_is_odd_man_out(player_index) )
        message = game_engine_message_odd_man_out;
    else if ( player->respawn_timer <= 0 )
        message = game_engine_message_waiting_for_space_to_clear;
    else
    {
        message = game_engine_message_respawn_timer;
        message_data = player->respawn_timer / 30;
    }

    return multiplayer_message_internal(player_index, message, message_data, buffer, buffer_length);
}
