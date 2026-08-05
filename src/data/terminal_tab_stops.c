/* terminal_tab_stops @0x82110B6C — .rdata, 8-byte span to the next symbol (aTerminalOutput
 * @0x82110B74); DB applied type `const __int16[3]`, so 6 bytes used + 2 bytes of alignment padding.
 * Big-endian halfwords:
 *   +0x0000 0x00A0 -> 160
 *   +0x0002 0x0140 -> 320
 *   +0x0004 0x01D6 -> 470
 *   +0x0006 0x0000 -> alignment padding (not an element)
 * Pixel tab-stop columns; terminal_draw passes the table with an explicit count of 3 to
 * draw_string_set_tab_stops for tab-stopped output lines. */

#include <stdint.h>

const int16_t terminal_tab_stops[3] = { 160, 320, 470 };
