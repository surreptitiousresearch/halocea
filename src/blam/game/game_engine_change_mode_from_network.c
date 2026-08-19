/* game_engine_change_mode_from_network @0x8374A888 — (client only) apply a "game engine change mode"
 * message-delta: mode 1 starts the 7-second postgame delay (game-over sound, close all UI widgets),
 * modes 2/3 advance to the postgame rasterize-delay / rasterize stages. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/game_engine_globals.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void ui_widgets_close_all(void);
extern void game_engine_progress_to_postgame_rasterize_delay(void);
extern void game_engine_progress_to_postgame_rasterize(void);

void game_engine_change_mode_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    int new_mode;
    if ( !message_delta_processor_decode_stateless(&new_mode, header) )
        return;

    switch ( new_mode )
    {
    case game_engine_mode_postgame_delay:
        game_engine_globals.game_ending_timer = 7.0f;
        game_engine_globals.mode = game_engine_mode_postgame_delay;
        game_engine_play_multiplayer_sound(_multiplayer_sound_game_over, 0);
        ui_widgets_close_all();
        break;
    case game_engine_mode_postgame_rasterize_delay:
        game_engine_progress_to_postgame_rasterize_delay();
        break;
    case game_engine_mode_postgame_rasterize:
        game_engine_progress_to_postgame_rasterize();
        break;
    }
}
