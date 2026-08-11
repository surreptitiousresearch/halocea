/* virtual_keyboard_tab_up @0x837836D0 — moves keyboard focus up, wrapping from row 0 to row 4, skipping
 * over any row whose key code (at the current column) matches the starting row's key code. Mirrors
 * virtual_keyboard_tab_left.c. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern const char virtual_keyboard_layout_table[5][11];
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t virtual_keyboard_tab_up(void)
{
    char start_key_code = virtual_keyboard_layout_table[virtual_keyboard_globals.row][virtual_keyboard_globals.column];
    int16_t row = virtual_keyboard_globals.row;

    do
    {
        --row;
        virtual_keyboard_globals.row = row;
        if ( row < 0 )
        {
            row = 4;
            virtual_keyboard_globals.row = 4;
        }
    }
    while ( virtual_keyboard_layout_table[row][virtual_keyboard_globals.column] == start_key_code );

    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
    return 1;
}
