/* screen_flash_type_1 @ 0x8417D7B4 (.data, 4 bytes)
 * DB applied_types: int screen_flash_type_1;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000006
 * Blam .lib data-global storage definition (init batch 7).
 */
#include "../headers/render_screen_flash_type.h"
/* screen_flash_type_1 @0x8417D7B4, 4-byte int = 6 (_render_screen_flash_type_tint). Consumer
 * inline extern aliases it `__int16`; DB extent is 4 (int), defined as int to match value. */
int screen_flash_type_1 = _render_screen_flash_type_tint; /* 6 */
