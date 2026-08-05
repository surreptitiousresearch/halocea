/* virtual_keyboard_layout_table @0x82123520 — .rdata, 56-byte span to the next symbol; DB applied
 * type `const char[5][11]`, i.e. 55 bytes used + 1 byte of alignment padding. All 7 declaring TUs
 * agree on `const char[5][11]`, and the dimensions are the DB constants
 * NUMBER_OF_VIRTUAL_KEYBOARD_ROWS (5) / NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS (11).
 *
 * It is a 2-D grid, not a flat table: virtual_keyboard_tab_up/tab_down index it as
 * [row][column] directly, and virtual_keyboard_process_internal flattens the same object as
 * [0][column + NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS * row] for the horizontal walks.
 *
 * Each cell holds a virtual_key_code (DB anon enum, see headers/virtual_key_code.h). Wide keys
 * occupy several adjacent cells with the same code, which is exactly what the tab_* walks skip over
 * ("while (table[...] == start_key_code)"). virtual_keyboard_select's `key < '$' || key > '+'`
 * range test is the action-key range 36..43 = _vkey_done.._vkey_space.
 * Raw bytes in address order (row-major, 11 per row):
 *   row 0  24 00 01 02 03 04 05 06 07 08 09   done, digits 1-9 then 0
 *   row 1  25 0A 0B 0C 0D 0E 0F 10 11 12 13   shift, letters a-j
 *   row 2  26 14 15 16 17 18 19 1A 1B 1C 1D   caps, letters k-t
 *   row 3  27 1E 1F 20 21 22 23 28 28 28 28   symbols, letters u-z, 4-wide backspace
 *   row 4  2B 2B 2B 2B 2B 2B 2B 29 29 2A 2A   7-wide space, 2-wide left, 2-wide right
 *   +0x37  00                                 alignment padding (not a cell) */

#include "../headers/virtual_key_code.h"
#include "../headers/virtual_keyboard_event.h"

const char virtual_keyboard_layout_table[NUMBER_OF_VIRTUAL_KEYBOARD_ROWS][NUMBER_OF_VIRTUAL_KEYBOARD_COLUMNS] =
{
    { _vkey_done,    _vkey_1, _vkey_2, _vkey_3, _vkey_4, _vkey_5, _vkey_6, _vkey_7, _vkey_8, _vkey_9, _vkey_0 },
    { _vkey_shift,   _vkey_a, _vkey_b, _vkey_c, _vkey_d, _vkey_e, _vkey_f, _vkey_g, _vkey_h, _vkey_i, _vkey_j },
    { _vkey_caps,    _vkey_k, _vkey_l, _vkey_m, _vkey_n, _vkey_o, _vkey_p, _vkey_q, _vkey_r, _vkey_s, _vkey_t },
    { _vkey_symbols, _vkey_u, _vkey_v, _vkey_w, _vkey_x, _vkey_y, _vkey_z,
      _vkey_backspace, _vkey_backspace, _vkey_backspace, _vkey_backspace },
    { _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_space,
      _vkey_left,  _vkey_left,  _vkey_right, _vkey_right }
};
