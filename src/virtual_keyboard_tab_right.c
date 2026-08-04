/* virtual_keyboard_tab_right @0x83783660 — moves keyboard focus right, wrapping from column 10 to column 0,
 * skipping over any column whose key code matches the starting column's key code. Mirrors
 * virtual_keyboard_tab_left.c. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern const char virtual_keyboard_layout_table[5][11];
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t virtual_keyboard_tab_right(void)
{
    __int16 row = virtual_keyboard_globals.row;
    char start_key_code = virtual_keyboard_layout_table[row][virtual_keyboard_globals.column];
    __int16 column = virtual_keyboard_globals.column;

    do
    {
        column = (column + 1 == 11) ? 0 : column + 1;
        virtual_keyboard_globals.column = column;
    }
    while ( virtual_keyboard_layout_table[row][column] == start_key_code );

    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
    return 1;
}
