/* game_time_statistics @ 0x84420968 (.data, 40 bytes)
 * DB applied_types: game_time_statistics game_time_statistics;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 file                       = 0x00000000
 *   +0x04 active                     = 0x00
 *   +0x05 first_line                 = 0x00
 *   +0x08 last_milliseconds          = 0x00000000
 *   +0x0C frame_count                = 0x0000
 *   +0x0E total_milliseconds_elapsed = 0x0000
 *   +0x10 minimum_milliseconds_per_frame = 0x0000
 *   +0x12 maximum_milliseconds_per_frame = 0x0000
 *   +0x14 total_latency              = 0x0000
 *   +0x16 minimum_latency            = 0x0000
 *   +0x18 maximum_latency            = 0x0000
 *   +0x1A server_updates             = 0x0000
 *   +0x1C minimum_server_updates     = 0x0000
 *   +0x1E maximum_server_updates     = 0x0000
 *   +0x20 predicted_updates          = 0x0000
 *   +0x22 minimum_predicted_updates  = 0x0000
 *   +0x24 maximum_predicted_updates  = 0x0000
 * per-frame timing/latency accumulator (data-bss, 40 bytes).
 */
#include "../headers/game_time_statistics.h"

struct game_time_statistics game_time_statistics;
