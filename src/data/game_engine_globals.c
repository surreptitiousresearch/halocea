/* game_engine_globals @ 0x846DB320 (.data, 36 bytes)
 * DB applied_types: _game_engine_globals game_engine_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 flags                      = 0x00000000
 *   +0x04 current_team_to_add        = 0x00000000
 *   +0x08 game_ending_timer          = 0x00000000 -> 0f
 *   +0x0C post_game_options_fade     = 0x00000000 -> 0f
 *   +0x10 mode                       = 0x00000000
 *   +0x14 score_alpha                = 0x0000000000000000
 *   +0x1C map_cycle_countdown_enabled = 0x00
 *   +0x20 time_until_map_cycle_timeout_expires_seconds = 0x00000000 -> 0f
 * data-bss (36 bytes).
 */
#include "../headers/game_engine_globals.h"

game_engine_globals_t game_engine_globals;
