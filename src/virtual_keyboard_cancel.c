/* virtual_keyboard_cancel @0x83783808 — abort virtual-keyboard editing, restoring the caller's buffer to
 * its pre-edit contents. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"


extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void input_abstraction_set_enabled_mode_keyboard(uint8_t enabled);

uint8_t virtual_keyboard_cancel(void)
{
    virtual_keyboard_globals.active = 0;
    if ( virtual_keyboard_globals.text_buffer )
    {
        ustrncpy(virtual_keyboard_globals.text_buffer, virtual_keyboard_globals.original_buffer,
                 virtual_keyboard_globals.buffer_size >> 1);
        *(unsigned __int16 *)((char *)virtual_keyboard_globals.text_buffer
                               + (virtual_keyboard_globals.buffer_size & ~0x1u)  /* even byte count: wchar terminator slot */ - 2) = 0;
    }
    virtual_keyboard_globals.text_buffer = 0;
    virtual_keyboard_globals.original_buffer[0] = 0;
    virtual_keyboard_globals.buffer_saved = 0;
    ui_play_audio_feedback_sound(_ui_audio_feedback_back);
    input_abstraction_set_enabled_mode_keyboard(0);
    return 1;
}
