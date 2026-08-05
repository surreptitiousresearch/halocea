/* CAVEAT: the second row is all zeros in the shipped data — see the note below. */

/* indexed_controller_bitmaps @0x821231E4 — .rdata, 8-byte span to the next symbol (aLongest
 * @0x821231EC); DB applied type `const unsigned __int8[2][3]`, i.e. 6 bytes used + 2 bytes of
 * alignment padding before the following string literal.
 * Raw bytes in address order:
 *   +0x0000 03 05 04   row 0
 *   +0x0003 00 00 00   row 1
 *   +0x0006 00 00      alignment padding (not cells)
 * Byte elements, stride 3, exactly two rows read: splitscreen_pregame_status_screen_update walks it
 * with `lbz` loads off a pointer initialized to &indexed_controller_bitmaps[0][2] and advanced by 3
 * (disasm 0x8377EEC8/0x8377EED0/0x8377EEE0/0x8377EEEC), and the loop's strength-reduced bound is
 * base+8 == start+3*2. The three columns are selected by the entry's team: [-2] for team >= 2,
 * [-1] for team 0, [0] for team 1 — i.e. { no team, red, blue } in memory order.
 * Row 0's values are exactly the player-1 controller sprite frames
 * {_controller_player1_active_bitmap, _controller_player1_red_active_bitmap,
 *  _controller_player1_blue_active_bitmap}.
 *
 * CAVEAT: row 1 is {0,0,0} in the binary, i.e. _controller_inactive_bitmap for all three columns,
 * where the enum's player-2 triple {6,8,7} is what the pattern would predict. The bytes are
 * reproduced as they ship; this is data as found, not a reconstruction gap. */

#include <stdint.h>
#include "../headers/controller_sprite_frame.h"

const uint8_t indexed_controller_bitmaps[2][3] =
{
    { _controller_player1_active_bitmap,        /* no team */
      _controller_player1_red_active_bitmap,    /* team 0 */
      _controller_player1_blue_active_bitmap }, /* team 1 */
    { _controller_inactive_bitmap, _controller_inactive_bitmap, _controller_inactive_bitmap }
};
