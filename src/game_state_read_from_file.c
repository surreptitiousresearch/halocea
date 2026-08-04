/* game_state_read_from_file @0x8371B1F0 — restore the game state via the HCEX checkpoint reader.
 * Sole caller (game_state_revert @0x83684AC8) ignores r3 => attested void return; the binary
 * tail-calls hcex_checkpoint_read and its result is discarded. */

#include <stdint.h>
#include "headers/xbox_game_state_globals.h"

extern uint8_t hcex_checkpoint_read(void *buffer, unsigned int buffer_size);

void game_state_read_from_file(void)
{
    hcex_checkpoint_read(xbox_game_state_globals.buffer, xbox_game_state_globals.buffer_size);
}
