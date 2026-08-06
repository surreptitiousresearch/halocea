/* rasterizer_frame_statistics @ 0x8465E5C0 (.data, 28 bytes)
 * DB applied_types: rasterizer_frame_statistics_s rasterizer_frame_statistics;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 fps                        = 0x00000000 -> 0f
 *   +0x04 fps_sample_count           = 0x0000
 *   +0x06 pad                        = 0x0000
 *   +0x08 fps_average                = 0x00000000 -> 0f
 *   +0x0C fps_min                    = 0x00000000 -> 0f
 *   +0x10 fps_max                    = 0x00000000 -> 0f
 *   +0x14 frames_dropped_pct         = 0x00000000 -> 0f
 *   +0x18 unused                     = 0x00000000
 */
#include "../headers/rasterizer_frame_statistics_s.h"
rasterizer_frame_statistics_s rasterizer_frame_statistics;
