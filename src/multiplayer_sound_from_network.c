/* multiplayer_sound_from_network @0x837B6D28 — DEVIATION: the DB's `game_engine_play_multiplayer_sound`
 * symbol (2-arg) is a different function from the one actually called here; disasm_range(0x837B6D28,
 * 0x837B6D6C) shows a `bl _game_engine_play_multiplayer_sound` to 0x837B6960, whose own DB prototype takes
 * 3 args (player_index, index, should_replicate) — matching the 3 registers loaded at the call site. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
/* wrong-callee fix (trap h): binary bl targets the internal 3-arg _game_engine_play_multiplayer_sound,
 * not the public 2-arg thunk (which is li r3,-1 + tail b into it). */
extern void _game_engine_play_multiplayer_sound(int player_index, int index, uint8_t should_replicate);

void multiplayer_sound_from_network(message_delta_processor_header *header, network_game_client *client)
{
    int sound_index;

    if ( header->decoding_information->mode )
        message_delta_processor_discard_iteration_body(header);
    else if ( message_delta_processor_decode_stateless(&sound_index, header) )
        _game_engine_play_multiplayer_sound(-1, sound_index, 0);
}
