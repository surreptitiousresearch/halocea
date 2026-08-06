/* mp_sound_queue @ 0x844A5C40 (.data, 80 bytes)
 * DB applied_types: queued_mp_sound mp_sound_queue[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 player_index               = 0x00000000
 *        +0x04 index                      = 0x00000000
 *        +0x08 ticks_left                 = 0x00000000
 *        +0x0C should_replicate           = 0x00
 *   [ 1] +0x00 player_index               = 0x00000000
 *        +0x04 index                      = 0x00000000
 *        +0x08 ticks_left                 = 0x00000000
 *        +0x0C should_replicate           = 0x00
 *   [ 2] +0x00 player_index               = 0x00000000
 *        +0x04 index                      = 0x00000000
 *        +0x08 ticks_left                 = 0x00000000
 *        +0x0C should_replicate           = 0x00
 *   [ 3] +0x00 player_index               = 0x00000000
 *        +0x04 index                      = 0x00000000
 *        +0x08 ticks_left                 = 0x00000000
 *        +0x0C should_replicate           = 0x00
 *   [ 4] +0x00 player_index               = 0x00000000
 *        +0x04 index                      = 0x00000000
 *        +0x08 ticks_left                 = 0x00000000
 *        +0x0C should_replicate           = 0x00
 * mp_sound_queue — multiplayer sound queue (data-bss); DB 5*16=80, ledger 84 (next-gap overcount)
 */
#include "../headers/queued_mp_sound.h"

queued_mp_sound mp_sound_queue[5];
