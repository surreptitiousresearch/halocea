/* screen_flash_type @ 0x84175BE4 (.data, 4 bytes)
 * DB applied_types: int screen_flash_type;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000005
 * data-init int. Binary bytes 0x00000005 big-endian = 5
 * (= _render_screen_flash_type_invert). DB applied-type: `int screen_flash_type;`.
 * NOTE: some consumers (e.g. game_engine_update_teleporter.c) reference a decompiler alias
 * `screen_flash_type_1` typed `__int16`; the DB authority is a 4-byte int, defined here as such.
 */
#include "../headers/render_screen_flash_type.h"

int screen_flash_type = _render_screen_flash_type_invert; /* 5 */
