/* virtual_keyboard_tab_left @0x837835A0 — moves keyboard focus left, wrapping from column 0 to column 10,
 * skipping over any column whose key code matches the starting column's key code (merged/blank cells). */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern const char virtual_keyboard_layout_table[5][11];
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t virtual_keyboard_tab_left(void)
{
    int16_t row = virtual_keyboard_globals.row;
    char start_key_code = virtual_keyboard_layout_table[row][virtual_keyboard_globals.column];
    int16_t column = virtual_keyboard_globals.column;

    do
    {
        --column;
        virtual_keyboard_globals.column = column;
        if ( column < 0 )
        {
            column = 10;
            virtual_keyboard_globals.column = 10;
        }
    }
    while ( virtual_keyboard_layout_table[row][column] == start_key_code );

    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
    return 1;
}
