/* game_state_write_to_file @0x8371B1D8 — persist the game state via the HCEX checkpoint writer. The
 * do_autosave argument is unused in this release path (caller game_state_save still passes it in r3). */

#include <stdint.h>

#include "headers/xbox_game_state_globals.h"

extern uint8_t hcex_checkpoint_write(void *buffer, unsigned int buffer_size);

uint8_t game_state_write_to_file(uint8_t do_autosave)
{
    /* (uint8_t) models the caller-side byte normalization (clrlwi r11,r3,24 @0x83684958);
     * the binary tail-calls hcex_checkpoint_write. */
    return hcex_checkpoint_write(xbox_game_state_globals.buffer, xbox_game_state_globals.buffer_size);
}
