/* global_goal @ 0x84460F20 (.data, 1024 bytes)
 * DB applied_types: netgame_goal global_goal[32];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 position                   = 0x000000000000000000000000
 *        +0x0C in_use                     = 0x00
 *        +0x10 player_index               = 0x00000000
 *        +0x14 team_index                 = 0x0000
 *        +0x18 ignore_player_index        = 0x00000000
 *        +0x1C nav_index                  = 0x0000
 *   [ 1] +0x00 position                   = 0x000000000000000000000000
 *        +0x0C in_use                     = 0x00
 *        +0x10 player_index               = 0x00000000
 *        +0x14 team_index                 = 0x0000
 *        +0x18 ignore_player_index        = 0x00000000
 *        +0x1C nav_index                  = 0x0000
 *   [ 2] +0x00 position                   = 0x000000000000000000000000
 *        +0x0C in_use                     = 0x00
 *        +0x10 player_index               = 0x00000000
 *        +0x14 team_index                 = 0x0000
 *        +0x18 ignore_player_index        = 0x00000000
 *        +0x1C nav_index                  = 0x0000
 *   [ 3] +0x00 position                   = 0x000000000000000000000000
 *        +0x0C in_use                     = 0x00
 *        +0x10 player_index               = 0x00000000
 *        +0x14 team_index                 = 0x0000
 *        +0x18 ignore_player_index        = 0x00000000
 *        +0x1C nav_index                  = 0x0000
 *   [ 4] +0x00 position                   = 0x000000000000000000000000
 *        +0x0C in_use                     = 0x00
 *        +0x10 player_index               = 0x00000000
 *        +0x14 team_index                 = 0x0000
 *        +0x18 ignore_player_index        = 0x00000000
 *        +0x1C nav_index                  = 0x0000
 *   [ 5] +0x00 position                   = 0x000000000000000000000000
 *        +0x0C in_use                     = 0x00
 *        +0x10 player_index               = 0x00000000
 *        +0x14 team_index                 = 0x0000
 *        +0x18 ignore_player_index        = 0x00000000
 *        +0x1C nav_index                  = 0x0000
 *   ... 26 further elements elided; full hex in .sweep/data_image.tsv
 * 32 * 32 = 1024 (data-bss, 1024 bytes).
 */
#include "../headers/netgame_goal.h"

netgame_goal global_goal[32];
