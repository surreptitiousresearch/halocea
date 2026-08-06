/* frame_storage @ 0x844A7490 (.data, 176 bytes)
 * DB applied_types: light_volume_frame frame_storage;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 unused1                    = 0x00000000000000000000000000000000
 *   +0x10 offset_from_marker         = 0x00000000 -> 0f
 *   +0x14 offset_exponent            = 0x00000000 -> 0f
 *   +0x18 length                     = 0x00000000 -> 0f
 *   +0x1C length_unused              = 0x0000000000000000000000000000000000000000000000000000000000000000
 *   +0x3C radius_hither              = 0x00000000 -> 0f
 *   +0x40 radius_yon                 = 0x00000000 -> 0f
 *   +0x44 radius_exponent            = 0x00000000 -> 0f
 *   +0x48 radius_unused              = 0x0000000000000000000000000000000000000000000000000000000000000000
 *   +0x68 color_hither               = 0x00000000000000000000000000000000
 *   +0x78 color_yon                  = 0x00000000000000000000000000000000
 *   +0x88 color_exponent             = 0x00000000 -> 0f
 *   +0x8C brightness_exponent        = 0x00000000 -> 0f
 *   +0x90 color_unused               = 0x0000000000000000000000000000000000000000000000000000000000000000
 * data-bss zero-fill; DB/consumer light_volume_frame (176).
 */
#include "../headers/light_volume_frame.h"

light_volume_frame frame_storage;
