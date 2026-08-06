/* global_screenshot_count @ 0x8441E42C (.data, 2 bytes)
 * DB applied_types: __int16 global_screenshot_count;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0000 (2 trailing bytes)
 * screenshot sequence counter (data-bss).
 * DEVIATION: was `int`. Every access in the binary is a SIGNED halfword — the readers all pair
 * `lhz` with `extsh` (0x8368AA0C+0x8368AA10 main_taking_screenshot, 0x8368B4C8 main_present_frame,
 * 0x8368C70C main_game_render, 0x83791E8C screenshot_in_progress, ...) and the writers use `sth`
 * (0x8368B90C screenshot_render, 0x8368C704 main_game_render). applied_types agrees: `__int16`.
 * Big-endian target: the halfword the binary touches is bytes 0..1 of the slot, where an `int`
 * declaration put the value in bytes 2..3. Next named symbol is hcex_coop_online_test_local @ +4,
 * so bytes 2..3 are alignment padding and narrowing cannot clobber a neighbour.
 */
#include <stdint.h>

int16_t global_screenshot_count;
