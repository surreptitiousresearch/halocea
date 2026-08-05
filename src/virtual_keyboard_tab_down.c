/* virtual_keyboard_tab_down @0x8378378C — moves keyboard focus down, wrapping from row 4 to row 0, skipping
 * over any row whose key code (at the current column) matches the starting row's key code. Mirrors
 * virtual_keyboard_tab_up.c. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern const char virtual_keyboard_layout_table[5][11];
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t virtual_keyboard_tab_down(void)
{
    int16_t row = virtual_keyboard_globals.row;
    char start_key_code = virtual_keyboard_layout_table[row][virtual_keyboard_globals.column];

    do
    {
        row = (row + 1 == 5) ? 0 : row + 1;
        virtual_keyboard_globals.row = row;
    }
    while ( virtual_keyboard_layout_table[row][virtual_keyboard_globals.column] == start_key_code );

    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
    return 1;
}
