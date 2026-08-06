/* game_state_globals @ 0x8441DE50 (.data, 28 bytes)
 * DB applied_types: $CFCD6AE9FC011AC9A63B1394A3C3FE62 game_state_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 base_address               = 0x00000000
 *   +0x04 cpu_allocation_size        = 0x00000000
 *   +0x08 gpu_allocation_size        = 0x00000000
 *   +0x0C allocation_size_checksum   = 0x00000000
 *   +0x10 locked                     = 0x00
 *   +0x11 saved_game_valid           = 0x00
 *   +0x14 revert_time                = 0x00000000
 *   +0x18 header                     = 0x00000000
 * game-state globals (data-bss, 28 bytes).
 */
#include "../headers/game_state_globals.h"

struct game_state_globals game_state_globals;
