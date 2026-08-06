/* sound_channels @ 0x846DB720 (.data, 6144 bytes)
 * DB applied_types: sound_channel_datum sound_channels[256];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 sound_index                = 0x00000000
 *        +0x04 type_flags                 = 0x0000
 *        +0x08 estimated_tick_time        = 0x00000000 -> 0f
 *        +0x0C pitch                      = 0x00000000 -> 0f
 *        +0x10 playing_permutation        = 0x00000000
 *        +0x14 queued_permutation         = 0x00000000
 *   [ 1] +0x00 sound_index                = 0x00000000
 *        +0x04 type_flags                 = 0x0000
 *        +0x08 estimated_tick_time        = 0x00000000 -> 0f
 *        +0x0C pitch                      = 0x00000000 -> 0f
 *        +0x10 playing_permutation        = 0x00000000
 *        +0x14 queued_permutation         = 0x00000000
 *   [ 2] +0x00 sound_index                = 0x00000000
 *        +0x04 type_flags                 = 0x0000
 *        +0x08 estimated_tick_time        = 0x00000000 -> 0f
 *        +0x0C pitch                      = 0x00000000 -> 0f
 *        +0x10 playing_permutation        = 0x00000000
 *        +0x14 queued_permutation         = 0x00000000
 *   [ 3] +0x00 sound_index                = 0x00000000
 *        +0x04 type_flags                 = 0x0000
 *        +0x08 estimated_tick_time        = 0x00000000 -> 0f
 *        +0x0C pitch                      = 0x00000000 -> 0f
 *        +0x10 playing_permutation        = 0x00000000
 *        +0x14 queued_permutation         = 0x00000000
 *   [ 4] +0x00 sound_index                = 0x00000000
 *        +0x04 type_flags                 = 0x0000
 *        +0x08 estimated_tick_time        = 0x00000000 -> 0f
 *        +0x0C pitch                      = 0x00000000 -> 0f
 *        +0x10 playing_permutation        = 0x00000000
 *        +0x14 queued_permutation         = 0x00000000
 *   [ 5] +0x00 sound_index                = 0x00000000
 *        +0x04 type_flags                 = 0x0000
 *        +0x08 estimated_tick_time        = 0x00000000 -> 0f
 *        +0x0C pitch                      = 0x00000000 -> 0f
 *        +0x10 playing_permutation        = 0x00000000
 *        +0x14 queued_permutation         = 0x00000000
 *   ... 250 further elements elided; full hex in .sweep/data_image.tsv
 * 256 * 24 = 6144 (data-bss, 6144 bytes).
 */
#include "../headers/sound_channel_datum.h"

sound_channel_datum sound_channels[256];
