/* virtual_keyboard_backspace @0x83783E50 — delete the character before the insertion point, shifting the
 * remainder of the buffer left. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"


extern void *memmove(void *dest, const void *src, unsigned int count);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

void virtual_keyboard_backspace(void)
{
    if ( virtual_keyboard_globals.insertion_point > virtual_keyboard_globals.text_buffer )
    {
        int remaining_bytes = (char *)virtual_keyboard_globals.text_buffer + virtual_keyboard_globals.buffer_size
                             - (char *)virtual_keyboard_globals.insertion_point;
        if ( remaining_bytes >= 0 )
        {
            memmove(virtual_keyboard_globals.insertion_point - 1, virtual_keyboard_globals.insertion_point, remaining_bytes);
            *(unsigned __int16 *)((char *)virtual_keyboard_globals.text_buffer
                                   + (virtual_keyboard_globals.buffer_size & ~1u) - 2) = 0; /* align byte size down to a whole wchar, zero the last slot */
            --virtual_keyboard_globals.insertion_point;
        }
    }
    ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
}
