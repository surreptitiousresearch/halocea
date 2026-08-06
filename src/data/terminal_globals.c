/* terminal_globals @ 0x84429B34 (.data, 28 bytes)
 * DB applied_types: $7AD7041248B2319AC41E71AD83B3A2E8 terminal_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 initialized                = 0x00
 *   +0x04 output_lines               = 0x00000000
 *   +0x08 newest_output_line_index   = 0x00000000
 *   +0x0C oldest_output_line_index   = 0x00000000
 *   +0x10 input_state                = 0x00000000
 *   +0x14 insertion_point_visible    = 0x00
 *   +0x18 last_insertion_point_toggle = 0x00000000
 * terminal HUD globals (data-bss, 28 bytes).
 */
#include "../headers/terminal_globals.h"

terminal_globals_t terminal_globals;
